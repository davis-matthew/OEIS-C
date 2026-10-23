/*
 * A176820  Number of primes between 3*A171821(2n-1) and 3*A171821(2n).
 * https://oeis.org/A176820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p such that 3*A171821(2n-1) ≤ p ≤
 *            3*A171821(2n)
 * Range:     u8 n <= 8, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/*  Set up a sieve and a twin‑prime list so that we can quickly count primes   */
/*  in the required interval.  The code works for standard integer seq_t as */
/*  well as for the arbitrary‑precision build where seq_t is a class type.   */
/* -------------------------------------------------------------------------- */

static size_t sieve_limit = 0;          /* maximum value already sieved */
static uint8_t *is_prime = NULL;       /* 0/1 array, is_prime[i] true iff i is prime */

static seq_t *twin_seq = NULL;         /* array of terms of A171821 */
static size_t twin_len = 0;             /* number of elements stored */
static size_t twin_cap = 0;             /* capacity of twin_seq */

/* forward declarations */
static void sieve_upto(size_t limit);
static void rebuild_twin_seq(void);
static void ensure_twin_seq(size_t needed);
static void ensure_limit_for_upper(size_t max_value);

/* sieve of Eratosthenes up to 'limit' (inclusive) */
static void sieve_upto(size_t limit) {
    if (limit <= sieve_limit) return;          /* already enough */

    free(is_prime);
    is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) exit(1);

    /* initialise all as prime */
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    sieve_limit = limit;
}

/* rebuild the twin‑prime list from the current sieve */
static void rebuild_twin_seq(void) {
    if (!twin_seq) {
        twin_cap = 1024;
        twin_seq = (seq_t *)malloc(twin_cap * sizeof(seq_t));
        if (!twin_seq) exit(1);
    }

    twin_len = 0;
    for (size_t p = 2; p <= sieve_limit; ++p) {
        if (!is_prime[p]) continue;
        bool twin = false;
        if (p == 2) {
            twin = true;                    /* 2 is counted */
        } else {
            if (p >= 2 && is_prime[p - 2]) twin = true;
            if (!twin && p + 2 <= sieve_limit && is_prime[p + 2]) twin = true;
        }
        if (twin) {
            if (twin_len == twin_cap) {
                twin_cap <<= 1;
                twin_seq = (seq_t *)realloc(twin_seq, twin_cap * sizeof(seq_t));
                if (!twin_seq) exit(1);
            }
            twin_seq[twin_len++] = (seq_t)p;
        }
    }
}

/* ensure that at least 'needed' elements of A171821 are available */
static void ensure_twin_seq(size_t needed) {
    if (twin_len >= needed) return;

    size_t lim = sieve_limit ? sieve_limit : 1000;
    if (lim < 1000) lim = 1000;

    while (twin_len < needed) {
        lim <<= 1;                /* double the limit */
        sieve_upto(lim);
        rebuild_twin_seq();
    }
}

/* ensure the sieve limit is at least 'max_value' */
static void ensure_limit_for_upper(size_t max_value) {
    if (max_value <= sieve_limit) return;
    size_t new_lim = sieve_limit ? sieve_limit : 1000;
    while (new_lim < max_value) new_lim <<= 1;
    sieve_upto(new_lim);
    /* rebuild twin list to keep it consistent */
    rebuild_twin_seq();
}

/* -------------------------------------------------------------------------- */
/*  Public entry point:  seq_t a(idx_t n)                                    */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* out of offset, harmless */

    /* we need 2*n terms of A171821 */
    size_t need = (size_t)(2 * n);
    ensure_twin_seq(need);

    /* lower and upper bounds of the interval */
    seq_t low  = 3 * (seq_t)twin_seq[need - 2];
    seq_t high = 3 * (seq_t)twin_seq[need - 1];

    /* make sure we can test primality up to 'high' */
    ensure_limit_for_upper((size_t)high);

    /* count primes in [low, high] with the pre‑computed sieve */
    idx_t lo = (idx_t)low;
    idx_t hi = (idx_t)high;
    unsigned int cnt = 0;   /* count easily fits in unsigned int for our bounds */

    for (idx_t v = lo; v <= hi; ++v)
        if (is_prime[v]) ++cnt;

    return (seq_t)cnt;
}

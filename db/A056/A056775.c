/*
 * A056775  Numbers k such that phi(k+12) = phi(k) + 12.
 * https://oeis.org/A056775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 2197, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A056775: numbers k such that φ(k+12) = φ(k) + 12
   by searching k sequentially and evaluating φ via prime factorisation
   with a precomputed list of small primes.  This is essentially a
   brute‑force sieve approach. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime list ------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_len   = 0;
static bool    primes_ready = false;

/* simple sieve that fills prime_list up to `limit`
   (works for limit up to several millions). */
static void generate_primes_up_to(uint64_t limit) {
    char *is_comp = (char *)malloc((size_t)(limit + 1));
    if (!is_comp) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) is_comp[i] = 0;

    uint64_t sqrt_lim = 0;
    for (sqrt_lim = 2; sqrt_lim * sqrt_lim <= limit; ++sqrt_lim);
    for (uint64_t i = 2; i <= sqrt_lim; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* expect about limit / log(limit) primes */
    size_t est = (size_t)(limit / 10 + 10);
    prime_list = (uint64_t *)realloc(prime_list, est * sizeof(uint64_t));
    if (!prime_list) exit(1);
    prime_len   = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (prime_len == est) {          /* grow if needed */
                est = est * 2;
                prime_list = (uint64_t *)realloc(prime_list,
                                                est * sizeof(uint64_t));
                if (!prime_list) exit(1);
            }
            prime_list[prime_len++] = i;
        }
    free(is_comp);
}

/* make sure we have primes at least up to max_needed.  Here we pre‑
   generate a generous bound once. */
static void ensure_primes(void) {
    if (!primes_ready) {
        generate_primes_up_to(1000000);   /* 1 000 000 primes suffice for n≤1e12 */
        primes_ready = true;
    }
}

/* ---------- totient via factorisation ----------------------------- */
static uint64_t phi_value(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t result = n;
    uint64_t temp   = n;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)          /* remaining prime factor > sqrt(original n) */
        result -= result / temp;
    return result;
}

/* ---------- dynamic array for sequence --------------------------- */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* grow seq_arr so it can hold at least `need` elements */
static void ensure_seq_cap(size_t need) {
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!seq_arr) exit(1);
        seq_cap = new_cap;
    }
}

/* push value to seq_arr */
static void seq_append(seq_t val) {
    ensure_seq_cap(seq_len + 1);
    seq_arr[seq_len++] = val;
}

/* ---------- scanning state -------------------------------------- */
static uint64_t next_k = 1;   /* next k to test */

/* ensure we have at least `needed` terms in seq_arr */
static void ensure_terms(size_t needed) {
    ensure_primes();        /* prime list ready */
    while (seq_len < needed) {
        uint64_t k = next_k;
        uint64_t phi_k  = phi_value(k);
        uint64_t phi_k12 = phi_value(k + 12);
        if (phi_k12 == phi_k + 12)
            seq_append((seq_t)k);
        ++next_k;
    }
}

/* ---------- public function -------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n=0 */
    ensure_terms((size_t)n);             /* guarantee n terms available */
    return seq_arr[(size_t)n - 1];
}

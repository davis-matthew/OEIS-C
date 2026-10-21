/*
 * A098058  Prime(n) such that 4 does not divide the difference between
 *          prime(n) and prime(n+1).
 * https://oeis.org/A098058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the last prime of each run of consecutive primes having
 *            the same residue modulo 4, starting with 2. Computed by prime
 *            generation and run detection
 * Range:     u8 n <= 36, u16 n <= 3886, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


/* Integer square root (used for trial division). */
static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid == 0) { res = 0; break; }
        if (mid <= n / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* Static state shared between calls. */
static idx_t *primes = NULL;          /* array of prime values (idx_t)       */
static size_t primes_cap = 0, primes_cnt = 0;
static seq_t *seq = NULL;             /* array of sequence terms (seq_t)     */
static size_t seq_cap = 0, seq_cnt = 0;
static idx_t next_candidate = 3;      /* next odd integer to test for prime */

/* Append a value to the dynamic array of primes, reallocating if needed. */
static void append_prime(idx_t val)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) exit(1);/* allocation failure */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = val;
}

/* Append a value to the dynamic array of sequence terms, reallocating if needed. */
static void append_seq(seq_t val)
{
    if (seq_cnt == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_cnt++] = val;
}

/* Primality test by trial division using already‐found primes. */
static bool is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    idx_t lim = isqrt(n);
    for (size_t i = 1; i < primes_cnt; ++i) {  /* primes[0] == 2 */
        idx_t p = primes[i];
        if (p > lim) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure that the seq array contains at least n terms. */
static void ensure_n_terms(idx_t n)
{
    if (seq_cnt >= (size_t)n) return;

    /* Initialise prime list with 2 if not yet done. */
    if (primes_cnt == 0) {
        append_prime(2ULL);
        /* 2 is the first term of the sequence. */
        if (seq_cnt == 0) append_seq((seq_t)2ULL);
    }

    while (seq_cnt < (size_t)n) {
        /* Generate the next odd candidate. */
        idx_t cand = next_candidate;
        next_candidate += 2;  /* only odd numbers after 2 */

        if (is_prime_candidate(cand)) {
            append_prime(cand);

            /* Detect the end of a run: when the residue mod 4 changes. */
            if (primes_cnt >= 2) {
                idx_t prev = primes[primes_cnt - 2];
                idx_t curr = primes[primes_cnt - 1];
                if ((prev & 3) != (curr & 3) && prev != 2ULL) {
                    append_seq((seq_t)prev);
                }
            }
        }
    }
}

/* Public function defined for OEIS. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n==0, but safe fallback */
    ensure_n_terms(n);
    return seq[n - 1];
}

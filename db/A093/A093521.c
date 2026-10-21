/*
 * A093521  Runs of 1's of lengths 1, prime(1), prime(2), prime(3), ...
 *          separated by 0's.
 * https://oeis.org/A093521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n lies inside one of the successive runs of * 1’s of
 *            lengths 1,2,3,5,7,… (the primes), otherwise a(n) = 0. * Runs are
 *            separated by a single 0. Sequence is indexed with offset 1 *
 *            (a(1)=1). The routine generates the primes on demand and * scans
 *            the runs until the requested position is...
 * Range:     u8 n <= 7672, u16 n <= 7672, u32 n <= 7672, u64 n <= 7672,
 *            u128 n <= 7672, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Prime list generated lazily. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t prime_cnt  = 0;         /* number of primes stored   */
static size_t prime_alloc = 0;        /* allocated array size      */

/* Ensure that at least 'need' primes are available (index 0‑based). */
static void ensure_prime_count(size_t need)
{
    while (prime_cnt < need) {
        /* Find next prime after the last stored one. */
        uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
        while (true) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_list[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (is_prime) break;
            ++cand;
        }

        /* Grow storage if necessary. */
        if (prime_cnt == prime_alloc) {
            size_t new_cap = (prime_alloc == 0) ? 4 : prime_alloc * 2;
            uint64_t *tmp = (uint64_t*)realloc(prime_list,
                                               new_cap * sizeof(uint64_t));
            prime_list = tmp;
            prime_alloc = new_cap;
        }

        prime_list[prime_cnt++] = cand;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, offset is 1 */

    /* Special handling of the first run of length 1 (position 1). */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;          /* first separating zero */

    /* Now analyze the remaining runs. */
    idx_t pos_zero = 2;          /* position of the last seen zero */
    size_t prime_idx = 0;        /* index of the next prime to use */

    while (true) {
        ensure_prime_count(prime_idx + 1);
        size_t run_len = prime_list[prime_idx++];
        idx_t run_start = pos_zero + 1;
        idx_t run_end   = run_start + run_len - 1;

        if (n >= run_start && n <= run_end) return (seq_t)1;

        pos_zero = run_end + 1;   /* zero after the run */
        if (n == pos_zero) return (seq_t)0;
    }
}

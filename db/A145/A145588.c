/*
 * A145588  a(n) = number of numbers removed in each step of Eratosthenes's
 *          sieve for 2^10.
 * https://oeis.org/A145588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = number of integers <= 2^10 removed at step n of
 *            Eratosthenes's sieve (i.e. the count of numbers <=1024 that are
 *            multiples of the nth prime and not divisible by any smaller
 *            prime, excluding the prime itself). Computed by simulating the
 *            sieve
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* static storage for precomputed values */
static seq_t *rem_counts = NULL;
static size_t rem_counts_len = 0;
static bool initialized = false;

/* One‑time initialization: generate primes up to 2^10 and the removal counts. */
static void init_rem_counts(void)
{
    const idx_t N = ((idx_t)1 << 10);          /* 2^10 = 1024 */
    /* marker array: true  -> number already removed (or composite) */
    bool *removed = (bool*)malloc((N + 1) * sizeof(bool));
    if (!removed) return;                     /* allocation failure → abort */
    memset(removed, 0, (N + 1) * sizeof(bool));

    /* temporary storage for the counts with a simple growth strategy */
    size_t cap = 200;                          /* initial capacity, enough for 2^10 */
    seq_t *counts_tmp = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!counts_tmp) { free(removed); return; }

    /* array of primes (not strictly needed afterwards, but used for the loop) */
    idx_t *prime_list = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!prime_list) { free(removed); free(counts_tmp); return; }

    size_t prime_cnt = 0;                      /* number of primes found */

    for (idx_t p = 2; p <= N; ++p) {
        if (!removed[p]) {
            /* p is prime */
            prime_list[prime_cnt++] = p;

            /* count numbers removed at this step */
            seq_t cnt = (seq_t)0;
            for (idx_t m = p; m <= N; m += p) {
                if (!removed[m]) {
                    if (m != p) {
                        cnt = cnt + (seq_t)1;
                    }
                    removed[m] = true;        /* mark as removed/composite */
                }
            }

            /* store the count, reallocate if needed */
            if (prime_cnt > cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(counts_tmp, cap * sizeof(seq_t));
                if (!tmp) break;            /* allocation failed → abort */
                counts_tmp = tmp;
            }
            counts_tmp[prime_cnt - 1] = cnt;
        }
    }

    /* copy final results into static storage */
    rem_counts = (seq_t*)malloc(prime_cnt * sizeof(seq_t));
    if (rem_counts) {
        memcpy(rem_counts, counts_tmp, prime_cnt * sizeof(seq_t));
    }
    rem_counts_len = prime_cnt;

    /* free temporary arrays */
    free(counts_tmp);
    free(removed);
    free(prime_list);
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_rem_counts();
        initialized = true;
    }
    /* n starts at 1; return 0 for any n > prime count or n == 0 */
    if (n == 0 || n > rem_counts_len) {
        return (seq_t)0;
    }
    return rem_counts[(size_t)(n - 1)];
}

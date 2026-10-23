/*
 * A145532  Number of numbers removed in each step of Eratosthenes's sieve for
 *          5!.
 * https://oeis.org/A145532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = number of integers removed in the n-th step of
 *            Eratosthenes's sieve when applied to the set {2,3,…,5!}. The
 *            steps correspond to the primes ≤ 120. a(1)=59, a(2)=19, a(3)=7,
 *            a(4)=4, …
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute number of numbers removed at step n (1‑based) for 5! = 120 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 5! = 120 */
    const idx_t N = 120;

    /* static storage for primes and counts, initialized once */
    static bool initialized = false;
    static idx_t primes[32];   /* enough for all primes <= 120 (there are 30) */
    static seq_t counts[32];
    static idx_t prime_cnt = 0;

    if (!initialized) {
        /* generate primes <= N using a simple sieve */
        bool is_composite[N + 1];
        for (idx_t i = 0; i <= N; ++i) is_composite[i] = false;
        for (idx_t p = 2; p * p <= N; ++p) {
            if (!is_composite[p]) {
                for (idx_t m = p * p; m <= N; m += p)
                    is_composite[m] = true;
            }
        }
        for (idx_t i = 2; i <= N; ++i)
            if (!is_composite[i]) primes[prime_cnt++] = i;

        /* simulate the sieve and count removals for each prime */
        bool removed[N + 1];
        for (idx_t i = 0; i <= N; ++i) removed[i] = false;
        for (idx_t k = 0; k < prime_cnt; ++k) {
            idx_t p = primes[k];
            seq_t removed_cnt = 0;
            for (idx_t m = 2 * p; m <= N; m += p) {
                if (!removed[m]) {
                    removed[m] = true;
                    removed_cnt++;
                }
            }
            counts[k] = removed_cnt;
        }
        initialized = true;
    }

    /* offset 1: a(1) is the first term */
    if (n == 0 || n > prime_cnt)
        return (seq_t)0;   /* undefined part of the sequence */
    return counts[n - 1];
}

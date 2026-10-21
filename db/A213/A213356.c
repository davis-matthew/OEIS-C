/*
 * A213356  Numbers that are not the sum of distinct primes with prime
 *          subscripts.
 * https://oeis.org/A213356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, fini, nonn
 * Formula:   a(n) = nth smallest positive integer that cannot be expressed *
 *            as a sum of distinct primes at prime indices
 *            (3,5,11,17,31,41,59,67,83). * The sequence is finite: every
 *            integer > 96 is expressible, so we pre‑compute * all
 *            non‑representable numbers ≤ 96 and store them in ascending order
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *nonrep = NULL;   /* cache of terms */
static size_t nonrep_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Predefined primes with prime indices (≤ 96) */
        const int primes[] = {3, 5, 11, 17, 31, 41, 59, 67, 83};
        const size_t P = sizeof(primes)/sizeof(primes[0]);

        /* reachable[s] == true iff s can be represented as sum of distinct primes */
        bool reachable[97] = {0};
        reachable[0] = true;

        for (size_t i = 0; i < P; ++i) {
            int p = primes[i];
            for (int s = 96; s >= p; --s) {
                if (reachable[s - p]) reachable[s] = true;
            }
        }

        /* Count how many numbers ≤ 96 are NOT reachable */
        size_t cnt = 0;
        for (int s = 1; s <= 96; ++s) {
            if (!reachable[s]) ++cnt;
        }

        /* Allocate cache and fill it */
        nonrep = (seq_t *)malloc(cnt * sizeof(seq_t));
        size_t pos = 0;
        for (int s = 1; s <= 96; ++s) {
            if (!reachable[s]) {
                nonrep[pos++] = (seq_t)s;
            }
        }
        nonrep_len = cnt;
        initialized = true;
    }

    /* If n is out of bounds, return 0 (out‑of‑range is unspecified) */
    if (n == 0 || n > nonrep_len) return (seq_t)0;
    return nonrep[n - 1];
}

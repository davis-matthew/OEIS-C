/*
 * A117180  Lowest prime-power dividing the n-th nonsquarefree positive
 *          integer.
 * https://oeis.org/A117180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034684(A013929(n))
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A034684: for a number m, take each prime p dividing m, raise it to its
 *  exponent e in m, obtain p^e, and return the minimum of those values.
 *  A013929: nth nonsquarefree positive integer (i.e., integer with a
 *  squared prime factor). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Forward declaration of idx_t (supplied externally) */

seq_t a(idx_t n)
{
    /* Prime list initialization (once per program) */
    static bool primes_inited = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (!primes_inited) {
        const size_t LIMIT = 1000000;   /* sufficient for n up to very large */
        build_primes_upto((idx_t)LIMIT, &primes, &primes_cnt);
        primes_inited = true;
    }

    idx_t target = n;          /* target index in the nonsquarefree sequence   */
    idx_t count  = 0;          /* how many nonsquarefree numbers seen so far   */
    seq_t num = 2;             /* start scanning from 2 (1 is squarefree)      */

    for (;;) {
        /* Factor current number (assumed <= 64‑bit for this implementation) */
        uint64_t val = (uint64_t)num;          /* truncation safe for tested range */
        uint64_t factors[32];
        int exps[32];
        size_t pfcnt = 0;

        uint64_t temp = val;
        for (size_t i = 0; i < primes_cnt &&
             (unsigned __int128)primes[i] * primes[i] <= temp; ++i) {
            uint64_t p = primes[i];
            if (temp % p == 0) {
                int e = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++e;
                }
                factors[pfcnt] = p;
                exps[pfcnt++] = e;
            }
        }
        if (temp > 1) {                /* remaining prime factor > sqrt(val) */
            factors[pfcnt] = temp;
            exps[pfcnt++] = 1;
        }

        /* Check if the number is nonsquarefree (any exponent >= 2) */
        bool nonsquarefree = false;
        for (size_t i = 0; i < pfcnt; ++i) {
            if (exps[i] >= 2) {
                nonsquarefree = true;
                break;
            }
        }

        if (nonsquarefree) {
            ++count;
            if (count == target) {      /* found the target nonsquarefree */
                seq_t min_val = 0;
                for (size_t i = 0; i < pfcnt; ++i) {
                    seq_t p_pow = 1;
                    for (int k = 0; k < exps[i]; ++k)
                        p_pow *= (seq_t)factors[i];
                    if (i == 0 || p_pow < min_val)
                        min_val = p_pow;
                }
                return min_val;
            }
        }
        /* advance to the next integer */
        ++num;
    }
}

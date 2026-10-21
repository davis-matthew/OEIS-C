/*
 * A081210  In prime factorization of n replace each prime power p^e with the
 *          greatest squarefree number <= p^e.
 * https://oeis.org/A081210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative and for a prime power p^e, a(p^e) is the
 *            greatest squarefree integer ≤ p^e. For n = ∏p_i^e_i, a(n) =
 *            ∏a(p_i^e_i)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two_seq, is_squarefree, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/pow_uint.h"
#include "../utils/is_power_of_two_seq.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;

    /* Simple trial‑division factorisation of n (works for n up to 2^64).  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;

    /* factor 2 separately */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            e++;
        }
        primes[cnt] = 2;
        exps[cnt] = e;
        ++cnt;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {  /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t N = pow_uint(p, (idx_t)e);  /* p^e as seq_t */

        if (N == (seq_t)p) {                 /* e == 1 -> squarefree already */
            result *= N;
        } else {
            seq_t cand = N;
            while (!is_squarefree(cand)) {
                cand--;                       /* decrement until squarefree */
            }
            result *= cand;
        }
    }
    return result;
}

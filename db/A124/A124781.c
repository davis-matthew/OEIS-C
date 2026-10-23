/*
 * A124781  a(n) = gcd(A093101(n), A093101(n+2)) where A093101(n) = gcd(n!,
 *          A(n)) and A(n) = A000522(n) = Sum_{k=0..n} n!/k!.
 * https://oeis.org/A124781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A093101(n), A093101(n+2)) * The algorithm uses the
 *            identity * A(n+1) = (n+1)*A(n) + 1 * and checks all divisors d
 *            of n+3 (a(n) always divides n+3). * For each divisor d it
 *            verifies that d divides * - n! (fact_n) * - A(n) (P_n) * -
 *            (n+2)! (fact_n2) * - A(n+2) (P_{n+2}) * The greatest di...
 * Range:     u8 n <= 366, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build list of divisors of m = n+3. */
    uint64_t m = (uint64_t)n + 3ULL;
    uint64_t divs[200];
    int dcnt = 0;
    for (uint64_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
        {
            divs[dcnt++] = i;
            if (i * i != m)
                divs[dcnt++] = m / i;
        }
    }

    seq_t best = 1;                          /* a(0) = 1, always divisor 1 */
    for (int di = 0; di < dcnt; ++di)
    {
        uint64_t d = divs[di];
        /* Compute n!  mod d and A(n)  mod d. */
        uint64_t fact_mod = 1 % d;           /* n! */
        uint64_t P_mod   = 1 % d;           /* A(n) = sum_{k=0}^n n!/k!  */
        for (idx_t k = 1; k <= n; ++k)
        {
            fact_mod = (fact_mod * (uint64_t)k) % d;
            P_mod    = (P_mod * (uint64_t)k + 1ULL) % d;
        }
        if (fact_mod != 0 || P_mod != 0)
            continue;                       /* d must divide both dvs */

        /* Compute (n+2)! mod d and A(n+2) mod d from the values above. */
        uint64_t fact_n2 = fact_mod;
        fact_n2 = (fact_n2 * ((uint64_t)n + 1ULL)) % d;
        fact_n2 = (fact_n2 * ((uint64_t)n + 2ULL)) % d;

        uint64_t P_next = ((uint64_t)n + 1ULL) * P_mod % d;
        P_next = (P_next + 1ULL) % d;                    /* P(n+1) mod d */
        uint64_t P_n2   = ((uint64_t)n + 2ULL) * P_next % d;
        P_n2 = (P_n2 + 1ULL) % d;                       /* P(n+2) mod d */

        if (fact_n2 == 0 && P_n2 == 0)
            if (d > (uint64_t)best)
                best = (seq_t)d;
    }
    return best;
}

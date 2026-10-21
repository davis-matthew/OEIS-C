/*
 * A090580  Number of numbers with 5 decimal digits and sum of digits = n.
 * https://oeis.org/A090580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A090580: number of 5‑digit natural numbers whose digit sum is n.
   For 1 ≤ n ≤ 45 the value is
       a(n) = Σ (-1)^|S| · C(n-1-Σ(L_i+1)+4 , 4)
   where the sum runs over all subsets S of {1,…,5} and
   L_1 = 8 (the first digit 1–9 → y₁ ∈ [0,8]) while L_i = 9 for i≥2.
   (Outside this range a(n)=0.) */

#include <stdbool.h>
#include <stdint.h>

/* small binomial coefficient for 0 <= k <= 4; result fits in 64 bits
   and is cast to seq_t. */
static seq_t binom_small(uint64_t n, uint64_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i) / i;
    }
    return (seq_t)res;
}

/* Compute a(n) for the OEIS sequence A090580. */
seq_t a(idx_t n)
{
    /* pre‑compute the values 1..45 once */
    static bool built = false;
    static seq_t tbl[46];                     /* indices 0..45, we use 1..45 */

    if (!built)
    {
        const int upper[5] = {8, 9, 9, 9, 9};   /* maximum allowed value for each variable */

        for (int s = 0; s <= 44; ++s)          /* s = n‑1, ranges 0..44 */
        {
            seq_t total = binom_small((uint64_t)s + 4, 4);  /* unrestricted solutions */

            for (int mask = 1; mask < (1 << 5); ++mask)    /* inclusion–exclusion */
            {
                uint64_t subsum = 0;
                int bits = 0;
                for (int i = 0; i < 5; ++i)
                {
                    if (mask & (1 << i))
                    {
                        subsum += (uint64_t)(upper[i] + 1);
                        ++bits;
                    }
                }
                if ((uint64_t)s < subsum) continue;          /* no solutions for this mask */

                seq_t val = binom_small((uint64_t)(s - subsum) + 4, 4);
                if (bits & 1)
                    total -= val;          /* odd cardinality -> subtract */
                else
                    total += val;          /* even cardinality -> add */
            }

            tbl[s + 1] = total;                 /* store for n = s+1 */
        }
        built = true;
    }

    if (n < 1 || n > 45) return (seq_t)0;       /* values outside the range are 0 */
    return tbl[n];
}

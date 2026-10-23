/*
 * A122124  Numbers n such that 25 divides Sum[ Prime[k]^n, {k,1,n}].
 * https://oeis.org/A122124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers m such that 25 divides Σ_{k=1}^{25} Prime[k]^m.
   The function returns the n‑th such exponent (1‑based). */
#include <stdint.h>
#include <stdbool.h>

/* the 25 primes < 100 */
static const uint32_t primes25[25] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};

/* for each residue r (0 ≤ r < 20) the sum of p^r (mod 25) over all
   primes ≠ 5.  The prime 5 is handled separately during the search. */
static seq_t sum_non5[20];
static bool init_done = false;

/* initialise sum_non5[ ] once; O(25⋅20) operations. */
static void init_sum_non5(void)
{
    if (init_done) return;
    for (int i = 0; i < 20; ++i) sum_non5[i] = 0;

    for (int pi = 0; pi < 25; ++pi) {
        uint32_t p = primes25[pi];
        if (p == 5) continue;                /* 5 handled later */
        int pmod = (int)(p % 25);
        int pow   = 1;                       /* p^0      */
        for (int r = 0; r < 20; ++r) {
            sum_non5[r] = (sum_non5[r] + pow) % 25;
            pow = (pow * pmod) % 25;         /* next power */
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0 */

    init_sum_non5();

    idx_t    count = 0;
    idx_t    exp   = 1;                      /* current exponent */

    while (true) {
        int r = (int)(exp % 20);             /* 0 ≤ r < 20 */
        seq_t sum = sum_non5[r];

        if (exp == 1)                        /* add 5^1 from the prime 5 */
            sum = (sum + 5) % 25;

        if (sum == 0) {                      /* divisible by 25 */
            ++count;
            if (count == n) return (seq_t)exp;
        }
        ++exp;
    }
}

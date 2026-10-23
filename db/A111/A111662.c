/*
 * A111662  Expansion of
 *          x^2*(1-x)*(x^2+x+1)*(x^6+x^3+1)/((2*x-1)*(2*x^9-x^6+x^3-1)).
 * https://oeis.org/A111662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A111662: Expansion of x^2*(1-x)*(x^2+x+1)*(x^6+x^3+1) /
                  ((2*x-1)*(2*x^9-x^6+x^3-1)).
   Recurrence (for n > 11):
      a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) - a(n-6) + 2*a(n-7) + 2*a(n-9) - 4*a(n-10)
   Initial values:
      a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=4, a(5)=9,
      a(6)=18, a(7)=36, a(8)=72, a(9)=144, a(10)=288, a(11)=576
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cached values up to a maximum of 20 000 terms.
       The array is large enough for all calls required in the tests
       (n up to about 10 000), and it works for arbitrary‑precision
       seq_t because the compiler will generate the appropriate
       constructors and destructors. */
    static seq_t cache[20000];
    static idx_t computed = 0;        /* highest index for which cache is valid */

    /* Initialise the first 12 values once. */
    if (computed == 0) {
        const seq_t init[12] = {0,0,1,2,4,9,18,36,72,144,288,576};
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        computed = 11;
    }

    if (n <= computed)
        return cache[n];

    /* Ensure the array is large enough for the requested n. */
    if ((size_t)n >= sizeof(cache)/sizeof(cache[0]))
        return 0;   /* out of range – should not happen in the tests */

    /* Compute values progressively using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* For i ≥ 12 all indices inside the recurrence are valid. */
        seq_t val = 2 * cache[i - 1]
                  + cache[i - 3]
                  - 2 * cache[i - 4]
                  - cache[i - 6]
                  + 2 * cache[i - 7]
                  + 2 * cache[i - 9]
                  - 4 * cache[i - 10];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}

/*
 * A231616  G.f. satisfies: A(x) = (1 - 3*x*A(x))^2 * (4*A(x) - 3).
 * https://oeis.org/A231616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 16*n*(n+1)*(109*n-135)*a(n) =
   8*n*(3706*n^2-6443*n+2403)*a(n-1) -
   9*(5341*n^3-17297*n^2+15870*n-3744)*a(n-2) +
   162*(n-3)*(2*n-3)*(109*n-26)*a(n-3)
   with a(0)=1, a(1)=2, a(2)=17 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 0;

    /* enlarge cache if required */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise first terms */
    if (last == 0) {
        cache[0] = (seq_t)1;
        last = 0;
    }
    if (n == 0) return cache[0];

    if (last < 1) {
        cache[1] = (seq_t)2;
        last = 1;
    }
    if (n == 1) return cache[1];

    if (last < 2) {
        cache[2] = (seq_t)17;
        last = 2;
    }
    if (n == 2) return cache[2];

    /* compute missing terms */
#ifdef __cplusplus
    /* Use a wider type for intermediate arithmetic.
       If seq_t is an integral type use unsigned __int128;
       otherwise use seq_t itself (the arbitrary‑precision class). */
    using big_t = typename std::conditional<
                      std::is_integral<seq_t>::value &&
                      sizeof(seq_t) <= sizeof(unsigned __int128),
                      unsigned __int128,
                      seq_t>::type;

    for (idx_t i = last + 1; i <= n; ++i) {
        big_t nn   = (big_t)i;
        big_t a1   = (big_t)cache[i - 1];
        big_t a2   = (big_t)cache[i - 2];
        big_t a3   = (big_t)cache[i - 3];

        big_t coeff1 = (big_t)8 * nn * ((big_t)3706 * nn * nn
                                        - (big_t)6443 * nn + (big_t)2403);
        big_t coeff2 = (big_t)9 * ((big_t)5341 * nn * nn * nn
                                        - (big_t)17297 * nn * nn
                                        + (big_t)15870 * nn - (big_t)3744);
        big_t coeff3 = (big_t)162 * (nn - 3) * (2 * nn - 3) * ((big_t)109 * nn - 26);
        big_t denom  = (big_t)16 * nn * (nn + 1) * ((big_t)109 * nn - 135);

        big_t num = coeff1 * a1 - coeff2 * a2 + coeff3 * a3;
        cache[i] = (seq_t)(num / denom);
    }
#else
    /* Pure C: all integers are unsigned __int128 for safety */
    for (idx_t i = last + 1; i <= n; ++i) {
        unsigned __int128 nn   = (unsigned __int128)i;
        unsigned __int128 a1   = cache[i - 1];
        unsigned __int128 a2   = cache[i - 2];
        unsigned __int128 a3   = cache[i - 3];

        unsigned __int128 coeff1 = (unsigned __int128)8 * nn *
            ((unsigned __int128)3706 * nn * nn -
             (unsigned __int128)6443 * nn + (unsigned __int128)2403);
        unsigned __int128 coeff2 = (unsigned __int128)9 *
            ((unsigned __int128)5341 * nn * nn * nn -
             (unsigned __int128)17297 * nn * nn +
             (unsigned __int128)15870 * nn - (unsigned __int128)3744);
        unsigned __int128 coeff3 = (unsigned __int128)162 *
            (nn - 3) * (2 * nn - 3) *
            ((unsigned __int128)109 * nn - 26);
        unsigned __int128 denom =
            (unsigned __int128)16 * nn * (nn + 1) *
            ((unsigned __int128)109 * nn - 135);

        unsigned __int128 num = coeff1 * a1 - coeff2 * a2 + coeff3 * a3;
        cache[i] = (seq_t)(num / denom);
    }
#endif

    last = n;
    return cache[n];
}

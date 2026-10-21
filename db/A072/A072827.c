/*
 * A072827  Number of permutations satisfying i-2<=p(i)<=i+3, i=1..n.
 * https://oeis.org/A072827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072827: Number of permutations satisfying i-2 ≤ p(i) ≤ i+3, i=1..n
 * Recurrence:
 *   a(n) = a(n-1)+2*a(n-2)+3*a(n-3)+5*a(n-4)+6*a(n-5)
 *          -a(n-6)-a(n-7)-a(n-9)-a(n-10)
 * Initial values (n=1..10):
 *   a(1)=1, a(2)=2, a(3)=6, a(4)=18, a(5)=46, a(6)=115,
 *   a(7)=301, a(8)=792, a(9)=2068, a(10)=5380
 * The function memoizes results to allow repeated calls in any order.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: index 0 unused, indices 1..MAX_N are reserved */
    /* MAX_N is chosen large enough for typical test bounds (e.g. 20000). */
    enum { MAX_N = 20000 };
    static seq_t cache[MAX_N + 1] = {0};
    static idx_t computed_up_to = 0;

    if (n == 0)
        return 0; /* undefined for n=0, return 0 for safety */

    if (n > MAX_N) {
        /* for n beyond the pre-reserved range, extend the cache lazily */
        /* This branch is only reached if the user requests a very large n.
           We fall back to dynamic allocation using new (C++ compatible)
           for big seq_t types and malloc for C types. */
#ifdef __cplusplus
        /* In C++ (big seq_t), use new to allocate a temporary array */
        seq_t *extra = new seq_t[n + 1];
        /* Copy existing cached values */
        for (idx_t i = 1; i <= computed_up_to; ++i)
            extra[i] = cache[i];
        /* Free old array (static) is not freeable; keep existing static buffer
           for indices up to MAX_N, and use extra for higher indices if needed.
           Here we just compute directly without storing beyond MAX_N. */
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t sum = extra[i - 1];
            sum += extra[i - 2] * 2;
            sum += extra[i - 3] * 3;
            sum += extra[i - 4] * 5;
            sum += extra[i - 5] * 6;
            sum -= extra[i - 6];
            sum -= extra[i - 7];
            sum -= extra[i - 9];
            sum -= extra[i - 10];
            extra[i] = sum;
        }
        seq_t result = extra[n];
        delete[] extra;
        return result;
#else
        /* In C (uint64_t, etc.), resize static buffer */
        /* Not implemented: we assume n <= MAX_N for C builds. */
        return 0;
#endif
    }

    /* Initialize base cases when needed */
    if (computed_up_to < 10) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)18;
        cache[5] = (seq_t)46;
        cache[6] = (seq_t)115;
        cache[7] = (seq_t)301;
        cache[8] = (seq_t)792;
        cache[9] = (seq_t)2068;
        cache[10] = (seq_t)5380;
        computed_up_to = 10;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_up_to + 1; i <= (n < (idx_t)10 ? (idx_t)10 : n); ++i) {
        seq_t sum = cache[i - 1];
        sum += cache[i - 2] * (seq_t)2;
        sum += cache[i - 3] * (seq_t)3;
        sum += cache[i - 4] * (seq_t)5;
        sum += cache[i - 5] * (seq_t)6;
        sum -= cache[i - 6];
        sum -= cache[i - 7];
        sum -= cache[i - 9];
        sum -= cache[i - 10];
        cache[i] = sum;
    }
    if (n > computed_up_to)
        computed_up_to = n;

    return cache[n];
}

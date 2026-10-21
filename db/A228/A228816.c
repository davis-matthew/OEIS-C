/*
 * A228816  Sum of all parts of all partitions of n that contain 1 as a part.
 * https://oeis.org/A228816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A000041(n-1)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /*  offset is 1 – only n >= 1 is meaningful */
    if (n == (idx_t)0) return (seq_t)0;

    idx_t target = n - (idx_t)1;          /* need partition p(target) */

#if __cplusplus
    /* dynamic array of partitions for C++ (std::vector) */
    static std::vector<seq_t> p(1);       /* at least one element for p(0) */
    static idx_t computed = 0;           /* highest computed index */

    /* ensure p has room */
    if (p.size() <= (size_t)target) {
        p.resize(target + 1, (seq_t)0);
    }

    /* initialise p(0) */
    if (p[0] == (seq_t)0) p[0] = (seq_t)1;
#else
    /* dynamic array of partitions for C (malloc/realloc) */
    static seq_t *p = NULL;
    static idx_t cached  = 0;    /* allocated size */
    static idx_t computed = 0;   /* highest computed index */

    if (target >= cached) {
        size_t newsize = cached ? cached * 2ULL : 1ULL;
        while (newsize <= (size_t)target) newsize <<= 1;
        seq_t *tmp = (seq_t *)realloc(p, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        p = tmp;
        cached = (idx_t)newsize;
    }
    if (p[0] == (seq_t)0) p[0] = (seq_t)1;
#endif

    /* compute missing partition numbers up to 'target' */
    for (idx_t i = computed + (idx_t)1; i <= target; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;

            seq_t term1 = p[i - pent1];
            if (k & 1) sum += term1; else sum -= term1;

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            seq_t term2 = p[i - pent2];
            if (k & 1) sum += term2; else sum -= term2;
        }
        p[i] = sum;
    }
    computed = target;

    /* a(n) = n * p(n-1) */
    return (seq_t)n * p[target];
}

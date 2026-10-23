/*
 * A029841  McKay-Thompson series of class 8E for the Monster group.
 * https://oeis.org/A029841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f. = (∏_{k>0} (1+q^(2k-1))/(1+q^(2k)))^4
 * Range:     i8 n <= 12, i16 n <= 38, i32 n <= 134, i64 n <= 484,
 *            i128 n <= 1798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients a(n) are obtained by computing the square root of the
   series A(q^2) and its reciprocal, then using
   A(q) = sqrt(A(q^2)) + 4*q / sqrt(A(q^2))  */

#include <stddef.h>   /* for size_t */

#ifdef __cplusplus
#include <vector>
#endif

/* External types seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* ----------- static cache for computed terms ----------- */
    static idx_t    cap = 0;   /* number of terms stored (>=1 after init) */

#ifdef __cplusplus
    static std::vector<seq_t> aseq;
    static std::vector<seq_t> sseq;   /* sqrt(A(q^2)) */
    static std::vector<seq_t> rseq;   /* reciprocal of sqrt(A(q^2)) */
#else
    static seq_t *aseq = NULL;
    static seq_t *sseq = NULL;
    static seq_t *rseq = NULL;
#endif

    /* ---------- initialise on first call ---------- */
    if (cap == 0)
    {
        cap = 1;
#ifdef __cplusplus
        aseq.resize(cap);
        sseq.resize(cap);
        rseq.resize(cap);
        aseq[0] = 1;          /* a(0) = 1 */
        sseq[0] = 1;          /* sqrt(1) = 1 */
        rseq[0] = 1;          /* 1 / 1 = 1 */
#else
        aseq = (seq_t *)malloc(sizeof(seq_t));
        sseq = (seq_t *)malloc(sizeof(seq_t));
        rseq = (seq_t *)malloc(sizeof(seq_t));
        aseq[0] = 1;
        sseq[0] = 1;
        rseq[0] = 1;
#endif
    }

    /* ---------- extend cache if needed ---------- */
    if (n >= cap)
    {
#ifdef __cplusplus
        size_t old_cap = cap;
        aseq.resize(n + 1);
        sseq.resize(n + 1);
        rseq.resize(n + 1);
#else
        size_t old_cap = (size_t)cap;
        aseq = (seq_t *)realloc(aseq, (n + 1) * sizeof(seq_t));
        sseq = (seq_t *)realloc(sseq, (n + 1) * sizeof(seq_t));
        rseq = (seq_t *)realloc(rseq, (n + 1) * sizeof(seq_t));
#endif
        /* compute terms from old_cap to n inclusive */
        for (idx_t i = old_cap; i <= n; ++i)
        {
            /* coefficient of q^i in A(q^2) */
            seq_t e = (i & 1) ? 0 : aseq[i / 2];

            /* s[i] : sqrt(A(q^2)) coefficient */
            seq_t sum_sconv = 0;
            for (idx_t k = 1; k < i; ++k)
                sum_sconv +=
#ifdef __cplusplus
                    sseq[k] * sseq[i - k];
#else
                    sseq[k] * sseq[i - k];
#endif
            sseq[i] = (e - sum_sconv) / 2;

            /* r[i] : coefficient of 1 / sqrt(A(q^2)) */
            seq_t sum_r = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum_r +=
#ifdef __cplusplus
                    sseq[k] * rseq[i - k];
#else
                    sseq[k] * rseq[i - k];
#endif
            rseq[i] = -sum_r;   /* since sseq[0] == 1 */

            /* a[i] = s[i] + 4 * r[i-1]   (for i>=1) */
            if (i >= 1)
                aseq[i] = sseq[i] + 4 * rseq[i - 1];
        }
        cap = n + 1;
    }

    return aseq[n];
}

/*
 * A103321  Expansion of 1 / ((1-x-x^2-x^3)*(1-x-x^3)).
 * https://oeis.org/A103321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-3)- 2*a(n-4)-a(n-5)-a(n-6) Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=18, a(5)=35
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#if defined(__cplusplus)
#  include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* --- static storage for memoised values -------------------------------- */
    static seq_t *seq = NULL;          /* array of computed terms */
    static idx_t   size = 0;           /* allocated size in entries   */
    static idx_t   max_calc = 5;       /* highest index pre‑computed    */

    /* --- pre‑computed initial terms --------------------------------------- */
    const seq_t init[6] = { (seq_t)1, (seq_t)2, (seq_t)4,
                            (seq_t)9, (seq_t)18, (seq_t)35 };

    /* --- trivial case ----------------------------------------------------- */
    if (n <= 5)
        return init[(size_t)n];

    /* --- grow storage if necessary ---------------------------------------- */
    if (n + 1 > size) {
        idx_t new_size = n + 1;
        if (seq == NULL) {
            seq = (seq_t *)malloc(sizeof(seq_t) * new_size);
        } else {
            seq = (seq_t *)realloc(seq, sizeof(seq_t) * new_size);
        }
        /* initialise newly allocated entries that fall in the initial range */
        for (idx_t i = size; i <= n; ++i) {
            if (i <= 5)
                seq[i] = init[(size_t)i];
            else
                seq[i] = 0;
        }
        size = new_size;
    }

    /* --- compute missing terms -------------------------------------------- */
    if (n > max_calc) {
        for (idx_t k = max_calc + 1; k <= n; ++k) {

#if defined(__cplusplus) && \
    (defined(__cpp_lib_is_integral) || defined(_MSC_VER))
            /* if seq_t is an integral type, use a 64‑bit intermediate to
               avoid overflow with small types (e.g. uint8_t)                     */
            if constexpr (std::is_integral<seq_t>::value) {
                unsigned long long t = (unsigned long long)seq[k-1] * 2ULL
                                      + (unsigned long long)seq[k-3];
                unsigned long long s = (unsigned long long)seq[k-4] * 2ULL
                                      + (unsigned long long)seq[k-5]
                                      + (unsigned long long)seq[k-6];
                seq[k] = (seq_t)(t - s);
            } else
#endif
            {
                /* otherwise (big‑integer classes) rely on precise
                   arithmetic of seq_t itself.  No overflow will occur   */
                seq_t t = seq[k-1] * ((seq_t)2);
                t += seq[k-3];
                seq_t s = seq[k-4] * ((seq_t)2);
                s += seq[k-5];
                s += seq[k-6];
                seq[k] = t - s;
            }
        }
        max_calc = n;
    }

    return seq[n];
}

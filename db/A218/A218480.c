/*
 * A218480  Number of 3n-length 10-ary words, either empty or beginning with
 *          the first letter of the alphabet, that can be built by repeatedly
 *          inserting triples of identical letters into the initially empty
 *          word.
 * https://oeis.org/A218480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n * Σ_{j=0..n-1} C(3n,j)*(n-j)*9^j for n>0, a(0) = 1
 *            The recurrence 2*n*(2*n-1)*(13*n-15)*a(n) =
 *            (55159*n^3-95963*n^2+38478*n-1080)*a(n-1)
 *            -27000*(3*n-5)*(3*n-4)*(13*n-2)*a(n-2) with a(0)=1, a(1)=1,
 *            a(2)=28 is employed below. For 64‑bit seq_t the computation is
 *            carried out with 128‑bit i...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <cstddef>
#endif

#ifdef __cplusplus
  /* C++17: switch on the size of seq_t */
  #if __has_include(<cstdint>)
    #include <cstdint>
  #endif
#endif

seq_t a(idx_t n)
{
    /* Treat the trivial cases explicitly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 28;

    seq_t prev2 = 1;      /* a(0) */
    seq_t prev1 = 28;     /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
    #if defined(__cplusplus)
        if constexpr (sizeof(seq_t) <= sizeof(uint64_t))
        {
            /* Use 128‑bit intermediates for small seq_t types */
            unsigned __int128 left   = (unsigned __int128)2 * i * (2*i - 1) * (13*i - 15);
            unsigned __int128 coeff_A = (unsigned __int128)55159 * i * i * i
                                       - (unsigned __int128)95963 * i * i
                                       + (unsigned __int128)38478 * i
                                       - 1080;
            unsigned __int128 coeff_B = (unsigned __int128)27000 * (3*i - 5)
                                       * (3*i - 4) * (13*i - 2);

            unsigned __int128 num = coeff_A * (unsigned __int128)prev1
                                    - coeff_B * (unsigned __int128)prev2;
            cur = (seq_t)(num / left);
        }
        else
        {
            /* Use the natural seq_t arithmetic for arbitrary‑precision types */
            seq_t left   = (seq_t)2 * i * (2*i - 1) * (13*i - 15);
            seq_t coeff_A = (seq_t)55159 * i * i * i
                            - (seq_t)95963 * i * i
                            + (seq_t)38478 * i
                            - 1080;
            seq_t coeff_B = (seq_t)27000 * (3*i - 5)
                            * (3*i - 4) * (13*i - 2);

            seq_t num = coeff_A * prev1 - coeff_B * prev2;
            cur = num / left;
        }
    #else
        /* Plain C: only 128‑bit intermediates are available */
        unsigned __int128 left   = (unsigned __int128)2 * i * (2*i - 1) * (13*i - 15);
        unsigned __int128 coeff_A = (unsigned __int128)55159 * i * i * i
                                   - (unsigned __int128)95963 * i * i
                                   + (unsigned __int128)38478 * i
                                   - 1080;
        unsigned __int128 coeff_B = (unsigned __int128)27000 * (3*i - 5)
                                   * (3*i - 4) * (13*i - 2);

        unsigned __int128 num = coeff_A * (unsigned __int128)prev1
                                - coeff_B * (unsigned __int128)prev2;
        cur = (seq_t)(num / left);
    #endif
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

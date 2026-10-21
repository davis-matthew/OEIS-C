/*
 * A071832  Frequency ratios for notes of C-major scale starting at c = 1
 *          (denominators).
 * https://oeis.org/A071832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = odd part of (base denominator × 2^k), where * k = ⌊n/7⌋
 *            and the base denominators correspond to the notes * in a C‑major
 *            scale: 1, 8, 4, 3, 2, 3, 8. The value is * obtained by reducing
 *            the fraction (base_num × 2^k)/(base_den) to * lowest terms and
 *            returning the denominator. Since the only...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base numerators and denominators of the C‑major just‑intonation ratios */
    static const seq_t base_num[7] = {
        (seq_t)1,  (seq_t)9,  (seq_t)5,  (seq_t)4,  (seq_t)3,  (seq_t)5,  (seq_t)15
    };
    static const seq_t base_den[7] = {
        (seq_t)1,  (seq_t)8,  (seq_t)4,  (seq_t)3,  (seq_t)2,  (seq_t)3,  (seq_t)8
    };

    /* Pre‑computed trailing‑zero counts (v₂) for the numerators and denominators */
    static const uint8_t num_p2[7] = {0, 0, 0, 2, 0, 0, 0};
    static const uint8_t den_p2[7] = {0, 3, 2, 0, 1, 0, 3};

    idx_t octave = n / 7;
    unsigned note  = (unsigned)(n % 7);

    /* The exponent of 2 that divides the final denominator:
     *   g = min( den_p2[n] , (num_p2[n] + octave) )   */
    uint8_t g;
    if (den_p2[note] > num_p2[note]) {
        uint64_t diff = (uint64_t)(den_p2[note] - num_p2[note]);

        /* If octave < diff then use num_p2 + octave,
           otherwise the denominator is already fully reduced */
        if (octave < diff) {
            g = (uint8_t)(octave + num_p2[note]);
        } else {
            g = den_p2[note];
        }
    } else {
        g = den_p2[note];
    }

    return base_den[note] >> g;
}

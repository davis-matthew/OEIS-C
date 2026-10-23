/*
 * A153646  Sum of the letter values of the words of the Sator square: SATOR
 *          AREPO TENET OPERA ROTAS.
 * https://oeis.org/A153646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   a(n) is the sum of the letter values (A=1 … Z=26) of the nth
 *            word * in the Sator square. The five words are SATOR, AREPO,
 *            TENET, OPERA, ROTAS. * The sequence repeats every 5 words
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 5 words of the Sator square */
    const char *const words[5] = {"SATOR", "AREPO", "TENET", "OPERA", "ROTAS"};
    static seq_t sums[5] = {0};
    static unsigned char init = 0;

    /* Initialise the sums once (closed over the words above). */
    if (!init) {
        for (int w = 0; w < 5; ++w) {
            seq_t s = 0;
            const char *p = words[w];
            while (*p) {
                /* *p is an uppercase letter (A‑Z). */
                s += (seq_t)(*p - 'A' + 1);
                ++p;
            }
            sums[w] = s;
        }
        init = 1;
    }

    /* Guard for n < 1 – it should not occur, but return 0 safely. */
    if (n == 0u) return 0;

    /* Compute (n-1) mod 5 to map 1‑based n into [0,4]. */
    size_t idx = (size_t)((n - 1u) % 5u);
    return sums[idx];
}

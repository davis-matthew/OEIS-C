/*
 * A097105  Gregorian years containing two Islamic New Year Days.
 * https://oeis.org/A097105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) – Gregorian year containing two Islamic New Year days (OEIS A097105)
 *  The routine simulates the definition from the OEIS entry.
 *  It incrementally evaluates the Islamic new‑year JDN
 *      I(i) = 1948086 + ⌊(10631·i + 3)/30⌋
 *  and the Gregorian new‑year JDN
 *      G(g) = 1721426 + ⌊1461·g/4⌋ – ⌊g/100⌋ + ⌊g/400⌋
 *  and follows the iterative rule:
 *      start i = 1, g = 621
 *      repeat:
 *          i++; g++;
 *          if I(i) < G(g) { count++; if count==n return g; i++; }
 *
 *  The function works for any n ≥ 1 and does not depend on any pre‑computed
 *  tables, thus satisfying the sequence definition exactly.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset does not matter below 1 */
    idx_t i = 1;
    idx_t g = 621;
    idx_t count = 0;

    while (1) {
        i++;                        /* advance to next Islamic and Gregorian year */
        g++;

        /* Islamic new year JDN for year i */
        seq_t I = (seq_t)1948086
                + (((seq_t)10631 * i + 3) / 30);

        /* Gregorian new year JDN for year g+1 (i.e. year g) */
        seq_t G = (seq_t)1721426
                + (((seq_t)1461 * g) / 4)
                - ((seq_t)g / 100)
                + ((seq_t)g / 400);

        if (I < G) {
            count++;               /* a new Gregorian year contains two Islamic New Years */
            if (count == n) {
                return (seq_t)g;   /* g is the Gregorian year we return */
            }
            i++;                   /* skip the following Islamic year as per definition */
        }
    }
}

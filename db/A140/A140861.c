/*
 * A140861  Decimal Goedelization of Heyting's 11 axioms for intuitionistic
 *          propositional logic.
 * https://oeis.org/A140861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base
 * Formula:   a(n) = decimal Gödel number of the n‑th Heyting axiom * (defined
 *            only for 1 ≤ n ≤ 11)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Gödel numbers are obtained by concatenating the decimal codes
 * of the symbols in the axioms as given in the OEIS entry. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Decimal encodings of the 11 Heyting axioms. */
    static const char *const godel_strings[11] = {
        "1791410",                    /* axiom 1 */
        "91420792410",                /* axiom 2 */
        "91720799141109241100",       /* axiom 3 */
        "991720492711007917",         /* axiom 4 */
        "2791720",                    /* axiom 5 */
        "91491720072",                /* axiom 6 */
        "1791620",                    /* axiom 7 */
        "91620792610",                /* axiom 8 */
        "99171104927110079916207110", /* axiom 9 */
        "31791720",                   /* axiom10 */
        "99172049173200731"           /* axiom11 */
    };

    /* Cache parsed numeric values to avoid repeated string scans. */
    static seq_t godel_values[11];
    static bool init = false;

    if (!init) {
        for (size_t i = 0; i < 11; ++i) {
            const char *p = godel_strings[i];
            seq_t v = 0;
            while (*p) {
                v = v * (seq_t)10 + (seq_t)(*p - '0');
                ++p;
            }
            godel_values[i] = v;
        }
        init = true;
    }

    if (n >= 1 && n <= 11)
        return godel_values[(size_t)(n - 1)];

    /* For indices outside the defined range return 0. */
    return (seq_t)0;
}

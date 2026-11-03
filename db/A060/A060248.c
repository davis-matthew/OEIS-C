/*
 * A060248  Numbers that begin with the letter "d" in Portuguese.
 * https://oeis.org/A060248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int is_d(seq_t x)
{
    seq_t high = x;
    /* Reduce to the highest 3‑digit group (units, thousands, millions, …). */
    while (high >= 1000)
        high /= 1000;

    /* Only the following groups begin with a word that starts with 'd': */
    /*   2    → "dois"
        10   → "dez"
        12   → "doze"
        16‑19→ "dezasseis", "dezassete", "dezoito", "dezenove"
        200‑299 → "duzentos"                         */
    if (high == 2)
        return 1;
    if (high == 10 || high == 12)
        return 1;
    if (high >= 16 && high <= 19)
        return 1;
    if (high >= 200 && high <= 299)
        return 1;
    return 0;
}
/* Sequence: numbers whose Portuguese word starts with the letter 'd'.
   The function a(n) returns the nth such number (offset 1). */

seq_t a(idx_t n)
{
    seq_t val   = 1;   /* candidate integer */
    idx_t count = 0;   /* how many d‑numbers found so far */

    while (1) {
        if (is_d(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}

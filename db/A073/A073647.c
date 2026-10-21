/*
 * A073647  In decimal representation of n, replace composite digits (4, 6, 8
 *          and 9) with their concatenated prime factorizations (22, 23, 222
 *          and 33).
 * https://oeis.org/A073647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal digits of n with each composite digit (4,6,8,9)
 *            replaced by its * prime factorisation as digits: 4→22, 6→23,
 *            8→222, 9→33. Digits 0,1,2,3,5,7 remain unchanged
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Mapping for each decimal digit. */
    static const char *map[10] = {
        "0",   /* 0 stays 0 */
        "1",   /* 1 stays 1 */
        "2",   /* 2 stays 2 */
        "3",   /* 3 stays 3 */
        "22",  /* 4 => 2·2 */
        "5",   /* 5 stays 5 */
        "23",  /* 6 => 2·3 */
        "7",   /* 7 stays 7 */
        "222", /* 8 => 2·2·2 */
        "33"   /* 9 => 3·3 */
    };

    /* Handle the trivial n == 0 case (not part of the OEIS offset but keeps the
     * function well‑defined). */
    if (n == 0)
        return 0;

    /* Extract decimal digits of n into an array (least significant first). */
    int digits[20];          /* 64‑bit integer has at most 20 decimal digits. */
    int len = 0;
    idx_t tmp = n;
    do {
        digits[len++] = tmp % 10;
        tmp /= 10;
    } while (tmp);

    /* Build the result by processing digits from most significant to least
     * significant and applying the required mapping. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        const char *m = map[d];
        for (const char *p = m; *p; ++p) {
            res = res * 10 + (seq_t)(*p - '0');
        }
    }

    return res;
}

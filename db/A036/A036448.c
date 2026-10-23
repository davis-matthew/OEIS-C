/*
 * A036448  Smallest positive number containing n e's when spelled out in US
 *          English.
 * https://oeis.org/A036448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(0) = 2. For n > 0 let k = n / 7 and r = n % 7. * prefix(r) =
 *            {317, 1, 3, 11, 17, 111, 117}[r] * a(n) = prefix(r) concatenated
 *            with (317) repeated k times, * except that if r == 0 the first
 *            317 is already the prefix, * so only (k-1) more blocks are
 *            appended
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)2;

    /* prefix strings according to remainder r */
    static const char *prefix_map[7] = {
        "317",   /* r = 0 */
        "1",     /* r = 1 */
        "3",     /* r = 2 */
        "11",    /* r = 3 */
        "17",    /* r = 4 */
        "111",   /* r = 5 */
        "117"    /* r = 6 */
    };

    idx_t k = n / 7;
    idx_t r = n % 7;
    const char *pref = prefix_map[r];

    /* Convert prefix to seq_t */
    seq_t value = 0;
    for (const char *p = pref; *p; ++p)
        value = value * 10 + (seq_t)(*p - '0');

    /* Number of 317 blocks to append after the prefix */
    idx_t blocks = k;
    if (r == 0) {
        if (blocks > 0)
            blocks--;   /* the initial 317 was already used */
    }

    const seq_t block_val = 317;
    const seq_t pow3 = 1000;        /* 10^3, always fits in seq_t */

    for (idx_t i = 0; i < blocks; ++i)
        value = value * pow3 + block_val;

    return value;
}

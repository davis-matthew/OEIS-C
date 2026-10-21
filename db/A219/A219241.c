/*
 * A219241  Distribution of letters in the name of the days of the week
 *          (English).
 * https://oeis.org/A219241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   a(n) = number of times the letter with standard code n appears
 *            in all the seven English names for the days of the week
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 1u || n > 26u) return 0;
    const char *days[7] = {
        "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday", "Sunday"
    };
    char target = (char)('a' + n - 1u);
    idx_t i, j;
    idx_t cnt = 0;
    for (i = 0; i < 7u; i++) {
        const char *s = days[i];
        for (j = 0; s[j]; j++) {
            char c = s[j];
            if (c >= 'A' && c <= 'Z')
                c += 'a' - 'A';
            if (c == target) cnt++;
        }
    }
    return (seq_t)cnt;
}

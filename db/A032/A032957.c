/*
 * A032957  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑7 representation
 *            has even digits at all odd positions (counting from the most
 *            significant digit, position index 0). This implementation uses a
 *            simple brute‑force search
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool good(seq_t x)
{
    /* Extract base‑7 digits into an array (least significant first). */
    seq_t digits[200];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = t % 7;
        t = t / 7;
    }
    if (len == 0) return true;   /* 0 is trivially acceptable */

    /* Check digits from most significant (position 0). */
    for (size_t pos = 0; pos < len; ++pos) {
        seq_t d = digits[len - 1 - pos];
        if (pos & 1) {           /* odd position */
            if (d % 2 != 0)      /* odd digit at an odd position */
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below the offset is irrelevant */

    seq_t x = 1;
    idx_t count = 0;
    while (1) {
        if (good(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}

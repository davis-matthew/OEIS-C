/*
 * A032956  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑6 digits * at odd
 *            positions (counting from 0 as the most‑significant digit) * are
 *            even. Computed by brute‑force search
 * Range:     u8 n <= 128, u16 n <= 9668, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true iff the base‑6 representation of x has even digits
 * at all odd indices (0‑based from the most significant digit). */
static bool is_good(seq_t x)
{
    /* We build the digits from least‑significant to most‑significant. */
    unsigned char digits[512];
    size_t len = 0;
    seq_t t = x;

    if (t == 0) /* 0 is not in the sequence */
        return false;

    while (t > 0) {
        digits[len++] = (unsigned char)(t % 6);
        t = t / 6;
    }

    /* Check odd positions from the most significant side. */
    for (size_t k = 0; k < len; ++k) {
        size_t pos_from_left = len - 1 - k;  /* most significant is pos 0 */
        if (pos_from_left & 1) {            /* odd index */
            if ((digits[k] & 1) != 0)        /* digit odd */
                return false;
        }
    }
    return true;
}

/* Return the n-th term of A032956 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* not used; offset is 1 */

    seq_t candidate = 1;
    idx_t found = 0;

    while (1) {
        if (is_good(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        candidate++;
    }
}

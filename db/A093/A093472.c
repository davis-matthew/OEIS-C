/*
 * A093472  (2n+1)-digit anti-palindromic numbers or numberdromes, whose first
 *          and last digits add to ten, second and next-to-last add to ten and
 *          so on with the central digit a 5.
 * https://oeis.org/A093472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (10^(m+1))*n + 5*10^m + (10^m - n) (OEIS A093472)
 * Range:     u8 n <= 0, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth (0‑based) (2k+1)-digit anti‑palindromic number with
 * middle digit 5 and each outer pair summing to 10.  The digits are
 * enumerated in lexicographic order without zeros. */

seq_t a(idx_t n)
{
    idx_t sum   = 0;  /* total count of numbers before the current block */
    idx_t power = 1;  /* 9^k for the current block size */
    idx_t k     = 0;  /* number of digits on one side of the 5 */

    /* Find k such that the nth number lies in the block of 9^k numbers. */
    while (1) {
        power *= 9;          /* 9^k */
        ++k;
        if (n < sum + power) break;
        sum += power;
    }

    idx_t offset = n - sum;  /* position within the current k‑digit block */

    /* Convert offset into base‑9 with digits 1‑9; store in reverse order. */
    int digits_rev[64]; /* enough for any n that fits in idx_t  */
    for (idx_t i = 0; i < k; ++i) {
        digits_rev[i] = (int)((offset % 9) + 1);
        offset /= 9;
    }

    seq_t result = 0;

    /* Most significant side (left half). */
    for (idx_t i = 0; i < k; ++i) {
        result = result * 10 + digits_rev[k - 1 - i];
    }
    /* The central digit 5. */
    result = result * 10 + 5;

    /* Right half: digits mirroring with 10 – d. */
    for (idx_t i = 0; i < k; ++i) {
        result = result * 10 + (10 - digits_rev[i]);
    }

    return result;
}

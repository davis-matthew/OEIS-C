/*
 * A204063  Decimal expansion of 2^607 - 1, the 14th Mersenne prime
 *          A000668(14).
 * https://oeis.org/A204063
 *
 * Offset:    183    (a(183) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 365, u16 n <= 365, u32 n <= 365, u64 n <= 365,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204063 (decimal expansion of 2^607-1)
 * Sequence definition: a(n) is the (n‑183)-th digit of the decimal
 * representation of 2^607−1 (most significant digit first).  The first
 * term is a(183)=5.
 *
 * Implementation: horizontally compute the 183‑digit decimal number
 * 2^607−1 by repeated base‑10 multiplication.  The array is cached
 * statically so repeated calls are O(1) after the first call.
 */

#include <stdbool.h>
#include <stddef.h>

/* external linkages provided by the build */
extern seq_t pow_int(seq_t base, idx_t exp);          /* unused but kept for reference */
extern seq_t seq_to_str(seq_t v, char *buf, size_t *len);   /* unused but kept for reference */

seq_t a(idx_t n)
{
    static bool initialized = false;
    /* 183 digits (max length of 2^607-1) */
    static int digits_rev[200];      /* little‑endian digits for calculation */
    static int digits_arr[183];      /* final digits in normal order */
    static size_t digits_len = 0;

    if (!initialized) {
        /* compute 2^607-1 */
        int temp[200];
        size_t len = 1;
        temp[0] = 1;                 /* current value = 1   (2^0)  */

        for (idx_t i = 0; i < 607; ++i) {
            int carry = 0;
            for (size_t j = 0; j < len; ++j) {
                int prod = temp[j] * 2 + carry;
                temp[j] = prod % 10;
                carry   = prod / 10;
            }
            while (carry) {
                temp[len++] = carry % 10;
                carry /= 10;
            }
        }
        /* subtract 1 */
        size_t j = 0;
        while (j < len && temp[j] == 0) {
            temp[j] = 9;
            ++j;
        }
        if (j < len) temp[j] -= 1;
        /* store for quick access (normal order) */
        digits_len = len;
        for (size_t k = 0; k < len; ++k)
            digits_arr[k] = temp[len - 1 - k];
        initialized = true;
    }

    /* if below offset, behaviour is unspecified per requirements */
    if (n < 183ULL)
        return 0;

    size_t idx = (size_t)(n - 183ULL);
    if (idx >= digits_len)  /* out of range, but caller should not request */
        return 0;
    return (seq_t)digits_arr[idx];
}

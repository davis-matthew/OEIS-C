/*
 * A078218  Smallest multiple of n that begins with the concatenation of the
 *          divisors of n (in increasing order).
 * https://oeis.org/A078218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = the smallest multiple of n whose decimal representation *
 *            starts with the concatenation of the divisors of n (in
 *            increasing order). * The algorithm builds the prefix, then
 *            searches for the smallest k such that * the required suffix is
 *            less than 10^k; it uses modular arithmetic to avoid * c...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 29, u64 n <= 107, u128 n <= 359,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* comparison function for qsort on uint64_t */
static int cmp_uint64(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x;
    uint64_t b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0 */

    /* ---------- enumerate divisors of n in increasing order ---------- */
    uint64_t divs[256];
    size_t dcnt = 0;

    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i) divs[dcnt++] = n / i;
        }
    }
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* ---------- build the concatenated prefix and its remainder modulo n ---------- */
    seq_t prefix = 0;
    idx_t   rem_n = 0;          /* prefix % n  (using idx_t to hold up to n) */

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d   = divs[i];
        int      dlen = digit_len(d);
        seq_t    p10  = pow10((idx_t)dlen);          /* 10^dlen as seq_t */
        prefix = prefix * p10 + (seq_t)d;           /* update prefix value      */
        rem_n = (rem_n * ((idx_t)p10) + d) % n;     /* update remainder modulo n */
    }

    /* ---------- search for the smallest k such that the suffix < 10^k ---------- */
    idx_t   pow10k_mod_n = 1 % n;   /* 10^k mod n   */
    seq_t  pow10k_seq   = 1;        /* 10^k as seq_t */

    while (1) {
        idx_t x = (n - (rem_n * pow10k_mod_n) % n) % n; /* suffix needed */

        if ((seq_t)x < pow10k_seq) {
            /* candidate = prefix * 10^k + x */
            return prefix * pow10k_seq + (seq_t)x;
        }

        /* move to next power of ten: k := k+1 */
        pow10k_mod_n = (pow10k_mod_n * 10) % n;
        pow10k_seq   = pow10k_seq * 10;
    }
}

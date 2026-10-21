/*
 * A055785  a(n) = smallest prime q of form q=-1+(p+1)*10^w, where p is n-th
 *          prime, or 0 if there is no such prime.
 * https://oeis.org/A055785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime q of the form q = -1 + (p+1)*10^w, where p
 *            is the n‑th prime. Return 0 if no such prime exists
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_sum_div3, get_prime, is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_from_str.h"
#include "../utils/digit_sum_div3.h"

/* largest w to try (upper bound for the number of trailing 9's) */
#define MAX_W 20000

seq_t a(idx_t n)
{
    if (n == 0) {
        seq_t zero = 0;
        return zero;          /* offset is 1 */
    }

    /* get the n‑th prime (1‑based) */
    uint32_t p = get_prime(n);
    /* quick filter: if digit sum of p is divisible by 3, no prime can occur */
    if (digit_sum_div3(p)) {
        seq_t zero = 0;
        return zero;
    }

    /* convert p to a string (decimal) */
    char prime_str[32];
    int prime_len = snprintf(prime_str, sizeof prime_str, "%u", p);
    if (prime_len <= 0) {
        seq_t zero = 0;
        return zero;
    }

    /* allocate buffer for candidate strings: prime + up to MAX_W 9's + NUL */
    size_t buf_size = (size_t)prime_len + MAX_W + 1;
    char *buf = (char *)malloc(buf_size);
    if (!buf) {
        seq_t zero = 0;
        return zero;
    }

    /* copy the prime digits into the buffer */
    memcpy(buf, prime_str, (size_t)prime_len);
    buf[prime_len] = '\0';          /* ensure NUL at current end */

    /* iterate over w = 1 .. MAX_W, appending one more trailing 9 each time */
    for (int w = 1; w <= MAX_W; ++w) {
        buf[prime_len + w - 1] = '9'; /* add a new '9' */
        buf[prime_len + w] = '\0';    /* terminate string */

        seq_t cand = seq_from_str(buf);   /* create big integer from string */
        if (is_prime_big(cand)) {
            free(buf);
            return cand;                 /* first prime of this form */
        }
        /* candidate is discarded automatically; no extra work needed */
    }

    /* no prime found within the bound */
    free(buf);
    seq_t zero = 0;
    return zero;
}

/*
 * A121042  Smallest divisor of n that is also contained in the decimal
 *          representation of n.
 * https://oeis.org/A121042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ d : d divides n and decimal representation of d *
 *            occurs as a substring in the decimal representation of n }
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition depends only on integer arithmetic and string comparison. */

#include <stdint.h>
#include <string.h>

/* Convert an unsigned integer of type idx_t to a null‑terminated decimal string.
 * The destination buffer must hold at least 21 bytes (max 20 digits for
 * uint64_t plus the terminating NUL). */
static void idx_to_str(idx_t v, char *buf)
{
    char tmp[21];
    int i = 0;

    if (v == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (v > 0) {
        tmp[i++] = (char)('0' + (v % 10));
        v /= 10;
    }

    /* reverse the temporary buffer into the destination */
    for (int j = 0; j < i; ++j)
        buf[j] = tmp[i - 1 - j];
    buf[i] = '\0';
}

/* Smallest divisor of n that is also present as a substring of n. */
seq_t a(idx_t n)
{
    /* According to OEIS, n >= 1. For n == 0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* String representation of n */
    char n_str[21];
    idx_to_str(n, n_str);

    /* The answer cannot be larger than n. */
    idx_t best = n;

    /* Enumerate divisors up to sqrt(n). */
    for (idx_t i = 1; (i * i) <= n; ++i) {
        if (n % i != 0)
            continue;

        /* Divisor i */
        if (i < best) {
            char d_str[21];
            idx_to_str(i, d_str);
            if (strstr(n_str, d_str) != NULL) {
                best = i;
                if (best == 1)
                    break; /* 1 is the minimal possible value */
            }
        }

        /* Complement divisor j = n / i, avoid double count when i == j */
        idx_t j = n / i;
        if (j != i && j < best) {
            char d_str[21];
            idx_to_str(j, d_str);
            if (strstr(n_str, d_str) != NULL) {
                best = j;
                if (best == 1)
                    break;
            }
        }
    }

    return (seq_t)best;
}

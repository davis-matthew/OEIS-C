/*
 * A214144  Number of 0..4 colorings of a 3X(n+1) array circular in the n+1
 *          direction with new values 0..4 introduced in row major order.
 * https://oeis.org/A214144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the empirical linear recurrence of order 15 (see
 *            OEIS A214144), with initial values a(1)=33, a(2)=514,
 *            a(3)=28278, a(4)=1101051, a(5)=47730973, a(6)=2000093424,
 *            a(7)=84746307320, a(8)=3577656763765, a(9)=151219261926083,
 *            a(10)=6389075487200098, a(11)=269978080214894342,
 *            a(12)=1140772...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   cmp_seq_t, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mul_small.h"      /* seq_t mul_small(seq_t, unsigned int)   */
#include "../utils/cmp_seq_t.h"      /* int cmp_seq_t(const void *, const void *) */

static const int64_t coeff[16] = {
    0,         /* unused, coeff[0] */
    29, 667, -3517, -48855, 260073, 877261,
    -6916959, 3304148, 50505862, -103934388,
    3498864, 137456208, -66186464, -37655040, 18842112
};

static const char *base_str[15] = {
    "0",
    "33",
    "514",
    "28278",
    "1101051",
    "47730973",
    "2000093424",
    "84746307320",
    "3577656763765",
    "151219261926083",
    "6389075487200098",
    "269978080214894342",
    "11407722800807664903",
    "482032317577423439601",
    "20368126430994060884676"
};

/* Parse an unsigned decimal string into a seq_t by repeated addition.
   This function does not rely on seq_from_str. */
static seq_t parse_uint(const char *s)
{
    seq_t val = 0;
    for (; *s; ++s) {
        /* multiply val by 10 by adding val to itself 9 more times */
        seq_t temp = val;
        for (int i = 0; i < 9; ++i) temp += val;
        val = temp;                      /* now val = 10 * previous val */
        val += (seq_t)(*s - '0');         /* add the digit */
    }
    return val;
}

/* Initially store the 15 base values into a static array. */
static int base_initialized = 0;
static seq_t base[15];

static void init_base(void)
{
    if (base_initialized) return;
    for (int i = 0; i < 15; ++i) {
        base[i] = parse_uint(base_str[i]);
    }
    base_initialized = 1;
}

seq_t a(idx_t n)
{
    init_base();

    /* Handle trivial cases directly. */
    if (n == 0) return base[0];                 /* a(0) = 0, used only in recurrence */
    if (n <= 14) return base[n];                /* known initial values upto a(14) */

    /* Buffer of the last 15 computed values.  buffer[0] = a(k-15), ..., buffer[14] = a(k-1) */
    seq_t buffer[15];
    for (int i = 0; i < 15; ++i) buffer[i] = base[i];   /* buffer contains a(0)..a(14) */

    /* Iterate from k = 15 upto n, computing each next term from the recurrence. */
    for (idx_t k = 15; k <= n; ++k) {
        seq_t pos = 0, neg = 0;          /* positive and negative partial sums */

        /* Sum of terms with positive coefficients. */
        for (int i = 1; i <= 15; ++i) {
            seq_t term = mul_small(buffer[15 - i],
                                   (unsigned int)(coeff[i] > 0 ? coeff[i] : -coeff[i]));
            if (coeff[i] > 0) pos += term;
            else              neg += term;
        }

        /* Compute the next term as pos - neg (pos >= neg is guaranteed by the recurrence). */
        seq_t next;
        if (cmp_seq_t(&pos, &neg) < 0) {
            /* In theory this should never happen; guard against wrap‑around. */
            next = 0;
        } else {
            next = pos - neg;
        }

        /* Shift buffer left to make room for the new value. */
        for (int j = 0; j < 14; ++j) buffer[j] = buffer[j + 1];
        buffer[14] = next;
    }

    /* After loop, buffer[14] holds a(n). */
    return buffer[14];
}

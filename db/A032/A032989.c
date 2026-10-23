/*
 * A032989  If d,e are consecutive digits of n in base 7, then |d-e|>=4.
 * https://oeis.org/A032989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032989:
   a(n) = the n-th positive integer whose base‑7 digits d_i satisfy
          |d_i - d_{i+1}| ≥ 4 for all adjacent pairs. */

#include <stddef.h>
#include <stdint.h>

static seq_t f[64][7];     /* f[len][digit] = # of length‑len strings
                              starting with digit (len≥1) */
static seq_t cum[64];     /* cum[len] = total # of valid numbers
                              with at most len digits (leading≠0) */
static size_t max_len = 0;
static int initialized = 0;

/* Two adjacent base‑7 digits are allowed iff |d-e| ≥ 4. */
static int allowed(int d, int e)
{
    int diff = d - e;
    if (diff < 0) diff = -diff;
    return diff >= 4;
}

/* Fill tables up to a length where cum[max_len] ≥ n.  */
static void ensure_tables(idx_t n)
{
    if (!initialized) {
        cum[0] = 0;
        initialized = 1;
    }

    seq_t target = (seq_t)n;
    while (cum[max_len] < target) {
        size_t len = max_len + 1;         /* new length to compute */

        if (len == 1) {
            for (int d = 0; d <= 6; ++d) f[1][d] = 1;
        } else {
            for (int d = 0; d <= 6; ++d) {
                seq_t sum = 0;
                for (int e = 0; e <= 6; ++e) {
                    if (allowed(d, e)) sum += f[len - 1][e];
                }
                f[len][d] = sum;
            }
        }

        /* Count numbers of exactly len digits (leading digit≠0). */
        seq_t total = 0;
        for (int d = 1; d <= 6; ++d) total += f[len][d];
        cum[len] = cum[len - 1] + total;

        ++max_len;
    }
}

/* Return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside definition */
    ensure_tables(n);

    seq_t target = (seq_t)n;

    /* Find length L of desired number. */
    size_t L = 1;
    while (cum[L] < target) ++L;

    seq_t k = target - cum[L - 1];   /* rank within length‑L numbers, 1‑based */

    uint8_t digits[64];
    size_t pos = 0;
    int prev_digit = -1;
    size_t remaining = L;

    /* Choose the leading digit (non‑zero). */
    for (int d = 1; d <= 6; ++d) {
        seq_t cnt = f[L][d];
        if (k > cnt) {
            k -= cnt;
        } else {
            digits[pos++] = (uint8_t)d;
            prev_digit = d;
            --remaining;
            break;
        }
    }

    /* Choose the remaining digits. */
    while (remaining > 0) {
        for (int e = 0; e <= 6; ++e) {
            if (!allowed(prev_digit, e)) continue;
            seq_t cnt = f[remaining][e];     /* sequences of length 'remaining' starting with e */
            if (k > cnt) {
                k -= cnt;
            } else {
                digits[pos++] = (uint8_t)e;
                prev_digit = e;
                --remaining;
                break;
            }
        }
    }

    /* Convert base‑7 digits to decimal. */
    seq_t value = 0;
    for (size_t i = 0; i < L; ++i) {
        value = value * 7 + digits[i];
    }
    return value;
}

/*
 * A122474  To find the next term, multiply the number obtained by reading the
 *          even digits in order by the number obtained by reading the odd
 *          digits in order.
 * https://oeis.org/A122474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=237169; for n>1, a(n)=E(a(n-1)) * O(a(n-1))
   (E(x) = integer formed by the even‑digit sequence of x,
    O(x) = integer formed by the odd‑digit sequence of x). */

#include <stdbool.h>
#include <stdint.h>

/* Helper: compute the next term from the current term. */
static seq_t next_term(seq_t x)
{
    seq_t even = 0;       /* number formed by even digits */
    seq_t odd  = 0;       /* number formed by odd  digits */
    bool even_seen = false, odd_seen = false;

    /* Extract digits of x into a small array in reverse order. */
    uint8_t digits[40];   /* 40 is enough for 128‑bit numbers (≤39 digits) */
    int dlen = 0;
    seq_t tmp = x;

    if (tmp == 0) {
        /* Special case: single digit 0 (even). */
        digits[dlen++] = 0;
    } else {
        while (tmp > 0) {
            digits[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Process digits from most significant to least significant. */
    for (int i = dlen - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        if ((d & 1) == 0) {           /* even digit */
            even = even * 10 + d;
            even_seen = true;
        } else {                      /* odd digit */
            odd = odd * 10 + d;
            odd_seen = true;
        }
    }

    /* Treat an empty part as 1 (e.g., 0 has no odd digits). */
    if (!even_seen) even = 1;
    if (!odd_seen)  odd  = 1;

    return even * odd;
}

/* Public function required by the problem. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1, guard anyway */

    seq_t cur = (seq_t)237169;    /* a(1) */

    if (n == 1)
        return cur;

    for (idx_t i = 2; i <= n; ++i)
        cur = next_term(cur);

    return cur;
}

/*
 * A110305  Factors of alternators which produce least alternating multiples.
 * https://oeis.org/A110305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = min{k ≥ 1 | n·k is an alternating integer}, a(n)=0 if n≡0
 *            (mod 20)
 * Range:     u8 n <= 21, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x has alternating digit parities in base‑10. */
static bool is_alternating(seq_t x)
{
    int last_parity = -1;                /* parity of previously examined digit, -1 means none yet */
    while (x != 0) {
        int digit   = (int)(x % 10);     /* current least‑significant digit */
        int parity  = digit & 1;         /* 0 for even, 1 for odd                  */
        if (last_parity != -1 && parity == last_parity)
            return false;                /* two consecutive parity same → not alternating */
        last_parity = parity;
        x /= 10;
    }
    return true;                         /* all adjacent digits alternate */
}

seq_t a(idx_t n)
{
    /* Special case: multiples of 20 can never be alternating. */
    if (n % 20 == 0)
        return 0;

    seq_t N = (seq_t)n;                   /* cast once for repeated use */
    for (idx_t k = 1; ; ++k) {
        seq_t m = N * (seq_t)k;           /* product n*k; safe in seq_t */
        if (is_alternating(m))
            return (seq_t)k;              /* minimal k found  */
    }
}

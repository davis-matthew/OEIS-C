/*
 * A151716  Positive integers whose Spanish names contain a prime number of
 *          letters.
 * https://oeis.org/A151716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) is the n-th positive integer whose Spanish name contains a
 *            prime number of letters
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Pre‑computed letter counts for the basic Spanish words. */
static const unsigned int unit_len[10]   = {0,3,3,4,6,5,4,5,4,5};
static const unsigned int teen_len[10]  = {4,4,4,5,7,6,9,10,9,10};
static const unsigned int tens_len[10]  = {0,0,6,7,8,9,7,7,7,7};
static const unsigned int hund_len[10]  = {0,0,10,11,13,10,11,11,11,11};

/* Letter count for a number in the range 1 … 999. */
static unsigned int len_below_1000(uint64_t n)
{
    if (n < 10) return unit_len[n];
    if (n < 20) return teen_len[n - 10];
    if (n < 30) return 6 + unit_len[n - 20];       /* veinti + unit */

    if (n < 100) {                                 /* tens */
        unsigned int t = n / 10;
        unsigned int u = n % 10;
        unsigned int l = tens_len[t];
        if (u) l += 1 + unit_len[u];              /* y + unit */
        return l;
    }

    /* 100 … 999 */
    unsigned int h = n / 100;
    unsigned int r = n % 100;
    unsigned int l = (h == 1) ? ((r == 0) ? 4 : 6) : hund_len[h]; /* cien / ciento / hundreds */

    if (r > 0) l += len_below_1000(r);
    return l;
}

/* Letter count for any positive integer, up to several million. */
static unsigned int len_number(uint64_t n)
{
    if (n < 1000) return len_below_1000(n);

    uint64_t th = n / 1000;          /* thousands, at most 999 for our limits */
    uint64_t r  = n % 1000;
    unsigned int l = (th == 1) ? 3 : len_below_1000(th) + 3; /* mil or xxx mil */
    if (r > 0) l += len_below_1000(r);
    return l;
}

/* Prime test for small integers (only up to ~200 is needed). */
static int is_prime_small(int x)
{
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (int i = 3; i * i <= x; i += 2)
        if (x % i == 0) return 0;
    return 1;
}

/* ------------------------------------------------------------ */
/*  The requested sequence function.                             */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, safeguard */

    uint64_t count  = 0;
    uint64_t number = 1;

    while (1) {
        unsigned int l = len_number(number);
        if (is_prime_small(l)) {
            ++count;
            if (count == n) return (seq_t)number;
        }
        ++number;
    }
}

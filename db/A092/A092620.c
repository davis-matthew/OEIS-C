/*
 * A092620  Numbers with exactly one prime digit.
 * https://oeis.org/A092620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A092620: numbers with exactly one prime digit (2,3,5,7)
   by brute‑force enumeration.  The function returns the n‑th term
   (a(1)=2, a(2)=3, …).  It is correct for any n ≥ 1 and uses only
   integer arithmetic. */

#include <stdbool.h>

/* Table to test if a decimal digit is prime. */
static const bool prime_digit[10] = {
    false, /* 0 */
    false, /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    false, /* 6 */
    true,  /* 7 */
    false, /* 8 */
    false   /* 9 */
};

/* Return true if x has exactly one prime decimal digit. */
static bool one_prime_digit(idx_t x)
{
    int cnt = 0;
    while (x > 0) {
        if (prime_digit[x % 10]) {
            ++cnt;
            if (cnt > 1) /* early exit */
                return false;
        }
        x /= 10;
    }
    return (cnt == 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t cur   = 1;
    idx_t count = 0;

    while (true) {
        if (one_prime_digit(cur)) {
            ++count;
            if (count == n)
                return (seq_t)cur; /* cast fits into seq_t */
        }
        ++cur;
    }
}

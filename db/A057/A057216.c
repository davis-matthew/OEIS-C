/*
 * A057216  To get next term, multiply by 17, add 1 and discard any prime
 *          factors < 17.
 * https://oeis.org/A057216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057216 – the 17x+1 map with all prime factors < 17 removed.
   Period = 22, a(0) = 61.  The function returns the nth term for any
   n ≥ 0. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Small primes less than 17. */
static const seq_t SMALL_PRIMES[6] = {2, 3, 5, 7, 11, 13};

/* Apply one step of the 17x+1 map.  X has no prime factors < 17 before
   entry, or those factors are all removed afterwards. */
static seq_t next_value(seq_t x)
{
    seq_t original = x;
    /* Remove all prime factors < 17. */
    for (int i = 0; i < 6; ++i) {
        seq_t p = SMALL_PRIMES[i];
        while (x % p == 0) {
            x /= p;
        }
    }

    /* If nothing was removed, we are in the "no small factor" branch. */
    if (x == original) {
        x = x * (seq_t)17 + (seq_t)1;
        /* Remove any newly introduced factors < 17. */
        for (int i = 0; i < 6; ++i) {
            seq_t p = SMALL_PRIMES[i];
            while (x % p == 0) {
                x /= p;
            }
        }
    }
    return x;
}

/* Periodic table.  The period is small (22), so a fixed small array
   is sufficient.  The period is generated lazily on first use. */
static seq_t PERIOD[64];
static idx_t PERIOD_LEN = 0;

seq_t a(idx_t n)
{
    if (PERIOD_LEN == 0) {
        /* Generate the full period starting from the first term. */
        seq_t cur = (seq_t)61;
        idx_t i = 0;
        do {
            PERIOD[i++] = cur;
            cur = next_value(cur);
        } while (cur != (seq_t)61 && i < 64);   /* safety cap */
        PERIOD_LEN = i;
    }
    return PERIOD[(size_t)(n % PERIOD_LEN)];
}

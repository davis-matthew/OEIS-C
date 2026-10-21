/*
 * A083498  a(n)= smallest multiple of n greater than n which has no digit in
 *          common with a(n-1).
 * https://oeis.org/A083498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, fini, full
 * Formula:   a(n) = smallest multiple of n greater than n * with no digit in
 *            common with a(n‑1)
 * Range:     u8 n <= 47, u16 n <= 129, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t cache[130] = {0};     /* cache[0] unused, cache[1..129] stored */
static uint16_t masks[130] = {0};  /* digit masks for each cached value */

/* compute digit mask for a number */
static uint16_t digit_mask(seq_t val)
{
    uint16_t mask = 0;
    while (val != 0) {
        uint8_t d = (uint8_t)(val % 10);
        mask |= 1u << d;
        val /= 10;
    }
    return mask;
}

/* the requested sequence */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* n=0 is outside the OEIS offset */
    if (n > 129) return 0;     /* the sequence terminates after a(129) */

    if (cache[n] != 0) return cache[n]; /* already computed */

    if (n == 1) {              /* base value a(1) = 2 */
        cache[1] = (seq_t)2;
        masks[1] = 1u << 2;
        return cache[1];
    }

    /* ensure a(n‑1) is computed */
    if (cache[n - 1] == 0) a(n - 1);

    seq_t prev = cache[n - 1];
    uint16_t prevMask = masks[n - 1];

    seq_t multiple = (seq_t)n * 2;   /* first multiple greater than n */
    while (1) {
        uint16_t mMask = digit_mask(multiple);
        if ((mMask & prevMask) == 0) {
            cache[n] = multiple;
            masks[n] = mMask;
            return multiple;
        }
        multiple += (seq_t)n;
    }
}

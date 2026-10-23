/*
 * A021455  Decimal expansion of 1/451.
 * https://oeis.org/A021455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ⌊10ⁿ / 451⌋ mod 10 – the n‑th decimal digit of 1/451
 *            (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define MAX_PERIOD 400   /* φ(451) = 400, the maximum possible period of 1/451 */

static seq_t digits[MAX_PERIOD];
static size_t period = 0;

/* Compute the repeating block of decimal digits for 1/451 once. */
static void init_digits(void)
{
    if (period != 0) return;           /* already initialized */

    uint64_t rem = 1;                  /* initial remainder for 1/451 */
    size_t i = 0;

    /* Standard long division: digit = (rem*10)/451; next remainder = (rem*10)%451 */
    for (;;)
    {
        uint64_t d = (rem * 10ULL) / 451ULL;   /* 0 ≤ d ≤ 9 */
        digits[i] = (seq_t)d;

        rem = (rem * 10ULL) % 451ULL;          /* update remainder */

        ++i;
        if (rem == 1ULL)                      /* cycle repeats */
        {
            period = i;                       /* period length */
            break;
        }
        if (i >= MAX_PERIOD)                 /* safety; should never happen */
        {
            period = i;
            break;
        }
    }
}

seq_t a(idx_t n)
{
    init_digits();
    if (period == 0) return 0;                /* safety fallback */
    return digits[(size_t)(n % period)];
}

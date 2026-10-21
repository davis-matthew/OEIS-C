/*
 * A206548  Period 12: repeat [1, 5, 11, 13, 17, 19, 19, 17, 13, 11, 5, 1].
 * https://oeis.org/A206548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206548: Modd21 of odd integers relatively prime to 21.
   Computable by generating the first 9 terms via the Modd21 map and
   extending them with the order‑9 linear recurrence
     a(n) = 2*a(n‑1) – 2*a(n‑2) + a(n‑3) – a(n‑6) + 2*a(n‑7) – 2*a(n‑8) + a(n‑9),
   for n > 9.  The sequence repeats every 12 terms. */
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t period[12];
static bool period_init = false;

static void init_period(void)
{
    /* generate the first 9 terms from the definition */
    seq_t seed[9];
    idx_t count = 0;
    seq_t cand = 1;
    const seq_t mod21 = 21;          /* 21 fits in any seq_t */

    while (count < 9) {
        /* candidate is odd; check coprime to 21 */
        if (gcd(cand, mod21) == 1) {
            seq_t r = cand % mod21;
            seed[count] = (r % 2 == 1) ? r : (mod21 - r);
            ++count;
        }
        cand += 2;
    }

    /* copy seeds into period */
    for (idx_t i = 0; i < 9; ++i)
        period[i] = seed[i];

    /* compute the remaining terms 10, 11, 12 via the recurrence */
    for (idx_t i = 9; i < 12; ++i) {
        /* term = 2*period[i-1] - 2*period[i-2] + period[i-3]
                - period[i-6] + 2*period[i-7] - 2*period[i-8] + period[i-9] */
        seq_t term = 2 * period[i - 1];
        term -= 2 * period[i - 2];
        term += period[i - 3];
        term -= period[i - 6];
        term += 2 * period[i - 7];
        term -= 2 * period[i - 8];
        term += period[i - 9];
        period[i] = term;
    }

    period_init = true;
}

seq_t a(idx_t n)
{
    if (!period_init)
        init_period();
    /* period repeats every 12 terms */
    idx_t idx = (n - 1) % 12;
    return period[idx];
}

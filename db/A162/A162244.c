/*
 * A162244  Years in which January first falls on a Sunday (since 2001).
 * https://oeis.org/A162244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162244: Years in which January first falls on a Sunday (since 2001) */
/* The sequence repeats every 400 years. We pre‑compute the 400‑year cycle once and
   then compute the n‑th element using arithmetic. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t cycle[400];
static size_t cycle_len = 0;
static bool cycle_init = false;

static void init_cycle(void)
{
    /* Jan 1 2001 was Monday → weekday 1 (0 = Sunday, 1 = Monday, …) */
    int weekday = 1;
    for (uint64_t year = 2001; year <= 2001 + 399; ++year)
    {
        if (weekday == 0)
            cycle[cycle_len++] = (seq_t)year;

        /* Days in previous year determine next year's weekday start */
        bool leap = ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
        weekday = (weekday + (leap ? 2 : 1)) % 7;
    }
    cycle_init = true;
}

seq_t a(idx_t n)
{
    if (!cycle_init)
        init_cycle();

    /* n is 1‑based; convert to 0‑based index */
    idx_t idx0 = n - 1;
    size_t block = (size_t)(idx0 / cycle_len);
    size_t pos   = (size_t)(idx0 % cycle_len);

    /* Shift the pre‑computed year by 400 · block years */
    uint64_t year = (uint64_t)cycle[pos] + 400ULL * block;
    return (seq_t)year;
}

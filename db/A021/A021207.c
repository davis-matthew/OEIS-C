/*
 * A021207  Decimal expansion of 1/203.
 * https://oeis.org/A021207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/203 */
static seq_t period[84];
static unsigned char initialized = 0;

static void init_period(void)
{
    if (initialized)
        return;
    seq_t r = 1;
    for (idx_t i = 0; i < 84; ++i) {
        unsigned long long m = (unsigned long long)r * 10ULL;
        period[i] = (seq_t)(m / 203ULL);
        r = (seq_t)(m % 203ULL);
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    init_period();
    return period[n % 84u];
}

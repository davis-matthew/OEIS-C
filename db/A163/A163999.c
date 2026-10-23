/*
 * A163999  Decimal expansion of the Faraday constant according to the 2019 SI
 *          system.
 * https://oeis.org/A163999
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   a(n) = the (n-5)-th digit of 96485.3321233100184
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 5, a(n) = 0 (undefined in OEIS) */

static const seq_t NUM_INT  = (seq_t)96485ULL;          /* integer part */
static const seq_t NUM_FRAC = (seq_t)3321233100184ULL; /* fractional part as integer */

static const seq_t POW10[14] = {
    (seq_t)1ULL,
    (seq_t)10ULL,
    (seq_t)100ULL,
    (seq_t)1000ULL,
    (seq_t)10000ULL,
    (seq_t)100000ULL,
    (seq_t)1000000ULL,
    (seq_t)10000000ULL,
    (seq_t)100000000ULL,
    (seq_t)1000000000ULL,
    (seq_t)10000000000ULL,
    (seq_t)100000000000ULL,
    (seq_t)1000000000000ULL,
    (seq_t)10000000000000ULL
};

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    const idx_t idx = n - 5;          /* position in the digit stream */

    if (idx < 5) {                    /* integer part digits */
        seq_t v = NUM_INT / POW10[4 - idx];
        return v % (seq_t)10;
    }
    if (idx < 18) {                   /* fractional part digits (13 digits) */
        const idx_t fidx = idx - 5;   /* 0 .. 12 */
        seq_t v = NUM_FRAC / POW10[12 - fidx];
        return v % (seq_t)10;
    }

    return 0;                         /* digits beyond the known expansion */
}

/*
 * A228163  Loschmidt constant n_0 in the 2019 SI system in unit m^(-3).
 * https://oeis.org/A228163
 *
 * Offset:    26    (a(26) is the first term)
 * Keywords:  cons, easy, nonn
 * Formula:   a(n) = digit (n-25) of the decimal expansion of the Loschmidt
 *            constant, 2.686780111... × 10^25
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  The denominator of the rational representation 101325/(1.380649e-23*273.15)  */
static const uint64_t D = 37712427435ULL;   /* 1,380,649 × 27,315 / 100,000,000 = 37,712,427,435 */

/* static state for memoisation */
static seq_t *digits = NULL;               /* array of digits already generated */
static size_t capacity = 0;                /* allocated capacity of digits[]   */
static size_t computed = 0;                /* number of digits stored         */
static uint64_t rem = 0;                   /* current remainder after integer part  */
static int initialized = 0;                /* initialisation flag              */

/* initialise the state by computing the integer part of the constant */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* allocate an initial capacity */
    capacity = 64;
    digits = (seq_t *)malloc(capacity * sizeof(seq_t));

    /* digits of N = 101325 × 10^31 (i.e. 1 0 1 3 2 5 followed by 31 zeros) */
    uint64_t r = 0;
    const int first_digits[6] = { 1, 0, 1, 3, 2, 5 };
    int i;
    int started = 0;  /* start emitting digits after a non‑zero quotient */
    for (i = 0; i < 6; ++i) {
        r = r * 10 + first_digits[i];
        uint64_t q = r / D;
        r = r % D;
        if (!started) {
            if (q != 0) {
                started = 1;
                digits[computed++] = (seq_t)q;
            }
        } else {
            digits[computed++] = (seq_t)q;
        }
    }
    for (i = 0; i < 31; ++i) {   /* the 31 trailing zeros */
        r = r * 10;            /* digit is 0 */
        uint64_t q = r / D;
        r = r % D;
        if (!started) {
            if (q != 0) {
                started = 1;
                digits[computed++] = (seq_t)q;
            }
        } else {
            digits[computed++] = (seq_t)q;
        }
    }
    /* after processing all 36 digits of N, r is N % D, the remainder for the fractional part */
    rem = r;
}

/* generate the next decimal digit after the current remainder */
static seq_t generate_next(void)
{
    rem = rem * 10;
    seq_t d = (seq_t)(rem / D);
    rem = rem % D;

    if (computed == capacity) {
        capacity *= 2;
        digits = (seq_t *)realloc(digits, capacity * sizeof(seq_t));
    }
    digits[computed++] = d;
    return d;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 26) return (seq_t)0;          /* undefined for indices below the offset */
    size_t pos = (size_t)(n - 25);        /* 1‑based position in the digit stream */

    init();

    /* generate digits on demand */
    while (pos > computed) {
        generate_next();
    }
    return digits[pos - 1];
}

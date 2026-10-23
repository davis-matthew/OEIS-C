/*
 * A021866  Decimal expansion of 1/862.
 * https://oeis.org/A021866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit n of the decimal expansion of 1/862
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      (offset 0, so a(0) is the first digit after the decimal point) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence value type and index type are supplied by the build.
   Here we use the provided typedefs seq_t and idx_t. */

static uint8_t *digit_cache = NULL;   /* stores digits 0‑9 */
static idx_t cache_len   = 0;         /* number of digits currently stored */
static idx_t cache_cap   = 0;         /* allocated capacity of digit_cache */
static idx_t next_needed = 0;         /* next digit index to compute */
static uint64_t remainder = 1;        /* current remainder before computing next digit */

/* Ensure the digit cache has capacity for at least (n+1) digits. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= n)
        new_cap <<= 1;

    uint8_t *new_buf = (uint8_t *)realloc(digit_cache, new_cap * sizeof(uint8_t));
    if (!new_buf) {
        /* In the unlikely event of a memory allocation failure, we abort. */
        abort();
    }
    digit_cache = new_buf;
    cache_cap = new_cap;
}

/* Compute digits from current next_needed up to and including n. */
static void compute_up_to(idx_t n)
{
    if (n < next_needed)
        return;  /* already computed */

    ensure_capacity(n);

    for (idx_t i = next_needed; i <= n; ++i) {
        remainder *= 10;                /* bring down next decimal digit */
        uint8_t digit = remainder / 862; /* integer division */
        remainder %= 862;                /* remainder for next step */
        digit_cache[i] = digit;
    }

    cache_len = n + 1;
    next_needed = n + 1;
}

/* Return the nth decimal digit of 1/862 (offset 0). */
seq_t a(idx_t n)
{
    /* Lazy computation with memoization. */
    compute_up_to(n);
    return (seq_t)digit_cache[n];
}

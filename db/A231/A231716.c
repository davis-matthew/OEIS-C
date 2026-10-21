/*
 * A231716  Numbers with restricted residue set factorial base representation:
 *          numbers n which can be formed as a sum n = du*u! + ... + d2*2! +
 *          d1*1!, where each di is in range 1..i and gcd(di,i+1)=1.
 * https://oeis.org/A231716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1015, u32 n <= 1015, u64 n <= 1015,
 *            u128 n <= 1015, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with restricted residue set factorial base representation:
   a(n) = the n-th integer that can be written as sum_{i=1..k} d_i·i! ,
          where 1 ≤ d_i ≤ i and gcd(d_i, i+1) = 1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Euclidean algorithm for unsigned int */
static inline unsigned int small_gcd(unsigned int a, unsigned int b)
{
    while (b) {
        unsigned int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compile‑time maximum number of factorial positions that we support.
   64 is ample for every uint64_t index because the cumulative count
   overflows UINT64_MAX long before this limit. */
#define MAXK 64

/* Precomputed tables, updated on first call. */
static bool init_done = false;
static uint8_t  allowed[MAXK + 1][MAXK + 1];   /* allowed digits for each position */
static unsigned char allowed_len[MAXK + 1];     /* number of allowed digits per position */
static seq_t   fact[MAXK + 1];                 /* factorials */
static idx_t   count_len[MAXK + 1];            /* product of allowed counts up to each position */
static idx_t   cum[MAXK + 1];                  /* cumulative count of numbers */

/* One‑time initialisation of tables. */
static void init_tables(void)
{
    fact[0]      = (seq_t)1;
    count_len[0] = (idx_t)1;
    cum[0]       = (idx_t)0;
    allowed_len[0] = 0;

    for (size_t i = 1; i <= MAXK; ++i) {
        unsigned int rad = 0;
        unsigned int i_plus = (unsigned int)(i + 1);

        /* Compute allowed digits for position i */
        for (unsigned int d = 1; d <= (unsigned int)i; ++d) {
            if (small_gcd(d, i_plus) == 1) {
                allowed[i][rad++] = (uint8_t)d;
            }
        }
        allowed_len[i] = (unsigned char)rad;

        /* factorial for position i */
        fact[i] = fact[i - 1] * (seq_t)i;

        /* count_len update with clamping at UINT64_MAX */
        unsigned __int128 mult = (unsigned __int128)count_len[i - 1] * rad;
        count_len[i] = (mult > UINT64_MAX) ? UINT64_MAX : (idx_t)mult;

        /* cumulative count with clamping */
        unsigned __int128 add = (unsigned __int128)cum[i - 1] + count_len[i];
        cum[i] = (add > UINT64_MAX) ? UINT64_MAX : (idx_t)add;
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence offset is 1 */
    if (!init_done) init_tables();

    /* Find minimal k such that cum[k] ≥ n */
    size_t k = 1;
    while (k <= MAXK && cum[k] < n) ++k;
    if (k > MAXK) return (seq_t)0; /* index too large for supported range */

    /* Offset within the k‑digit numbers (1‑based) */
    idx_t offset = n - cum[k - 1];
    /* zero‑based rank */
    idx_t rem = offset - 1;

    seq_t value = (seq_t)0;
    for (int pos = (int)k; pos >= 1; --pos) {
        idx_t lower = count_len[pos - 1];
        idx_t idx_digit = rem / lower;    /* selects the j‑th allowed digit */
        uint8_t digit = allowed[pos][(unsigned int)idx_digit];
        rem = rem % lower;
        value += fact[pos] * (seq_t)digit;
    }
    return value;
}

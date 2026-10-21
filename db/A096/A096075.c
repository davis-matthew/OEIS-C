/*
 * A096075  Least common multiple of first n 3-smooth numbers.
 * https://oeis.org/A096075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of the first n 3‑smooth numbers (constructed by
 *            merging multiples of 2 and 3). This implementation generates the
 *            smooth numbers in order, storing only their power exponents, and
 *            keeps cumulative maximum exponents to compute the required lcm
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 105, u64 n <= 370,
 *            u128 n <= 1385, big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp);   */
#include "../utils/pow3.h"   /* seq_t pow3(idx_t exp);   */

static idx_t   g_max_n = 0;     /* number of entries already computed   */
static size_t  g_cap   = 0;     /* current allocation capacity          */
static uint16_t *g_e2 = NULL;  /* exponent of 2 in each smooth number */
static uint16_t *g_e3 = NULL;  /* exponent of 3 in each smooth number */
static uint16_t *g_m2 = NULL;  /* cumulative max exponent of 2 up to i */
static uint16_t *g_m3 = NULL;  /* cumulative max exponent of 3 up to i */
static idx_t   g_i2 = 0;        /* next index to multiply by 2          */
static idx_t   g_i3 = 0;        /* next index to multiply by 3          */

/* ensure static tables are initialized and large enough */
static void init_if_needed(void)
{
    if (g_max_n != 0) return;          /* already initialized */

    /* initial capacity of 1 element (index 0) */
    g_cap = 1;
    g_e2  = (uint16_t*)malloc(g_cap * sizeof(uint16_t));
    g_e3  = (uint16_t*)malloc(g_cap * sizeof(uint16_t));
    g_m2  = (uint16_t*)malloc(g_cap * sizeof(uint16_t));
    g_m3  = (uint16_t*)malloc(g_cap * sizeof(uint16_t));

    g_e2[0] = g_e3[0] = 0;   /* 3‑smooth number 1 = 2^0 * 3^0 */
    g_m2[0] = g_m3[0] = 0;   /* lcm so far is 1 */

    g_i2 = g_i3 = 0;
    g_max_n = 1;
}

/* enlarge the arrays to hold at least n elements (0 … n‑1) */
static void grow_to(idx_t n)
{
    if (g_cap >= n) return;
    g_cap = n;
    g_e2 = (uint16_t*)realloc(g_e2, g_cap * sizeof(uint16_t));
    g_e3 = (uint16_t*)realloc(g_e3, g_cap * sizeof(uint16_t));
    g_m2 = (uint16_t*)realloc(g_m2, g_cap * sizeof(uint16_t));
    g_m3 = (uint16_t*)realloc(g_m3, g_cap * sizeof(uint16_t));
}

/* ------------------------------------------------------------------ */
/* Function required by the test harness                              */
seq_t a(idx_t n)
{
    if (n == 0)               /* safety guard – not used in the OEIS */
        return (seq_t)0;

    init_if_needed();         /* does this only on first call */

    /* Generate terms until we have computed up to index n‑1 (0‑based) */
    for (idx_t m = g_max_n; m < n; ++m) {
        grow_to(m + 1);            /* ensure arrays large enough */

        /* candidate exponents for multiplying by 2 and by 3 */
        uint16_t e2_2 = g_e2[g_i2] + 1;
        uint16_t e3_2 = g_e3[g_i2];

        uint16_t e2_3 = g_e2[g_i3];
        uint16_t e3_3 = g_e3[g_i3] + 1;

        /* compute the candidate values */
        seq_t val2 = pow2(e2_2) * pow3(e3_2);
        seq_t val3 = pow2(e2_3) * pow3(e3_3);

        uint16_t next_e2, next_e3;

        if (val2 < val3) {                /* choose val2 */
            next_e2 = e2_2;
            next_e3 = e3_2;
            ++g_i2;
        } else if (val2 > val3) {          /* choose val3 */
            next_e2 = e2_3;
            next_e3 = e3_3;
            ++g_i3;
        } else {                           /* duplicate */
            next_e2 = e2_2;
            next_e3 = e3_2;
            ++g_i2;
            ++g_i3;
        }

        /* store the new number's exponents */
        g_e2[m] = next_e2;
        g_e3[m] = next_e3;

        /* update cumulative maximum exponents */
        g_m2[m] = (g_m2[m-1] > next_e2) ? g_m2[m-1] : next_e2;
        g_m3[m] = (g_m3[m-1] > next_e3) ? g_m3[m-1] : next_e3;
    }

    g_max_n = n;                          /* all indices < n are now valid */

    /* return the lcm for the first n numbers */
    return pow2(g_m2[n-1]) * pow3(g_m3[n-1]);
}

/*
 * A061127  Number of degree-n permutations of order exactly 24.
 * https://oeis.org/A061127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F4(n) - F8(n) - F12(n) + F24(n) where Fi(n) is the number
 *            of permutations on n symbols whose cycle lengths belong to the
 *            set of divisors of i. The recurrence for Fi(n) is Fi(0) = 1
 *            Fi(n) = Σ_{k∈Div(i), k≤n} Fi(n-k) * ( (n-1)! / (n-k)! ) (n≥1)
 *            This follows from the standard cycle-counting a...
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper sets of divisors */
static const idx_t div4[]  = {1,2,4};
static const idx_t div8[]  = {1,2,4,8};
static const idx_t div12[] = {1,2,3,4,6,12};
static const idx_t div24[] = {1,2,3,4,6,8,12,24};
static const idx_t *div_sets[4] = {div4, div8, div12, div24};
static const size_t div_cnt[4] = {3,4,6,8};

/* Pre‑allocated arrays for factorials and the four Fi sequences */
static seq_t *fact   = NULL;
static idx_t  fact_len = 0;

static seq_t *g4 = NULL;
static seq_t *g8 = NULL;
static seq_t *g12 = NULL;
static seq_t *g24 = NULL;
static idx_t  g_len  = 0;

/* Ensure fact[] has entries up to N (inclusive) */
static void ensure_fact(idx_t N)
{
    if (N < fact_len) return;
    size_t new_size = (size_t)(N + 1);
    fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
    for (idx_t i = fact_len; i <= N; ++i) {
        if (i == 0) fact[i] = (seq_t)1;
        else fact[i] = fact[i-1] * (seq_t)i;
    }
    fact_len = N + 1;
}

/* Ensure all g_* arrays have entries up to N (inclusive) */
static void ensure_g(idx_t N)
{
    if (N < g_len) return;
    size_t new_size = (size_t)(N + 1);
    g4  = (seq_t *)realloc(g4 , new_size * sizeof(seq_t));
    g8  = (seq_t *)realloc(g8 , new_size * sizeof(seq_t));
    g12 = (seq_t *)realloc(g12, new_size * sizeof(seq_t));
    g24 = (seq_t *)realloc(g24, new_size * sizeof(seq_t));
    /* initialize new entries based on recurrence */
    for (idx_t i = g_len; i <= N; ++i) {
        if (i == 0) {
            g4[i] = g8[i] = g12[i] = g24[i] = (seq_t)1;
            continue;
        }
        /* compute factorial ratio (i-1)!/(i-k)! for each needed k and store to avoid recomputation */
        seq_t sum4 = 0, sum8 = 0, sum12 = 0, sum24 = 0;
        /* dividend (i-1)! */
        seq_t nom = fact[i-1];
        /* for each divisor set */
        for (size_t t = 0; t < 4; ++t) {
            const idx_t *s = div_sets[t];
            const size_t steps = div_cnt[t];
            seq_t *dst = (t == 0) ? g4 : (t == 1 ? g8 : (t == 2 ? g12 : g24));
            seq_t sum = 0;
            for (size_t j = 0; j < steps; ++j) {
                idx_t k = s[j];
                if (k > i) continue;
                /* ratio = (i-1)! / (i-k)! */
                seq_t denom = fact[i-k];
                seq_t ratio = nom / denom;
                sum += dst[i-k] * ratio;
            }
            if (t == 0) sum4 = sum;
            else if (t == 1) sum8 = sum;
            else if (t == 2) sum12 = sum;
            else sum24 = sum;
        }
        g4[i]  = sum4;
        g8[i]  = sum8;
        g12[i] = sum12;
        g24[i] = sum24;
    }
    g_len = N + 1;
}

/* Public function as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset starts at 1 */
    ensure_fact(n);
    ensure_g(n);
    /* a(n) = g4[n] - g8[n] - g12[n] + g24[n] */
    seq_t res = g4[n] - g8[n] - g12[n] + g24[n];
    return res;
}

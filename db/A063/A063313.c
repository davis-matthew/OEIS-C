/*
 * A063313  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          40 ).
 * https://oeis.org/A063313
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063313: Dimension of the space of weight n cuspidal newforms for Γ₁(40).
   Recurrence (for n > 8):
        a(n) = a(n-2) + a(n-4) – a(n-6)
   Initial values:
        a(2) = –1, a(3) = 19, a(4) = 44, a(5) = 67,
        a(6) = 94, a(7) = 117, a(8) = 144. */
#include <stdlib.h>

/* constants – the array dimensions must be integer compile‑time constants */
#define OFFSET     2          /* a(2) is the first defined term */
#define INIT_LEN   7          /* number of given initial values (a(2)…a(8)) */

/* array of the initial values */
static const seq_t INIT_VAL[INIT_LEN] = {
    (seq_t)-1, (seq_t)19, (seq_t)44, (seq_t)67,
    (seq_t)94, (seq_t)117, (seq_t)144
};

/* dynamic table for memoization */
static seq_t *tab          = NULL;  /* table[0] = a(2), table[1] = a(3), … */
static size_t   cap       = 0;      /* allocated size of tab */
static idx_t    last_n    = 0;      /* largest n for which a(n) is stored */

/* ensure tab has room for indices 0 .. need-1  (i.e. for a(OFFSET+need-1)) */
static void ensure_capacity(size_t need)
{
    if (cap >= need) return;
    size_t old_cap = cap;
    if (cap == 0) cap = 8;
    while (cap < need) cap <<= 1;
    seq_t *new_tab = (seq_t *)realloc(tab, cap * sizeof(seq_t));
    if (!new_tab) exit(1);          /* allocation failure – stop */
    if (old_cap < cap) {
        for (size_t i = old_cap; i < cap; ++i) new_tab[i] = 0;
    }
    tab = new_tab;
}

/* initialise the table with the known initial values */
static void init_table(void)
{
    if (tab) return;               /* already done */
    ensure_capacity(INIT_LEN);
    for (size_t i = 0; i < INIT_LEN; ++i) tab[i] = INIT_VAL[i];
    last_n = OFFSET + INIT_LEN - 1; /* a(8) stored */
}

/* compute all terms from last_n+1 up to n (inclusive) */
static void compute_upto(idx_t n)
{
    if (n <= last_n) return;
    init_table();
    ensure_capacity(n - OFFSET + 1); /* need slot for a(n) */
    for (idx_t k = last_n + 1; k <= n; ++k)
    {
        if (k < OFFSET) continue;              /* undefined, ignore */
        size_t i = (size_t)(k - OFFSET);       /* index of a(k) in table */
        if (k >= 9) {                         /* recurrence applies */
            tab[i] = tab[i-2] + tab[i-4] - tab[i-6];
        }
        /* for 2 ≤ k ≤ 8 the values are already in tab via INIT_VAL */
    }
    last_n = n;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < OFFSET) return 0;              /* undefined for n < 2 */
    compute_upto(n);
    return tab[n - OFFSET];
}

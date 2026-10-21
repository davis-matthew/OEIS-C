/*
 * A087181  Number partition numbers <= P(n) of the form 3*k+1 (P = A000041).
 * https://oeis.org/A087181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k : 0 ≤ k ≤ n , p(k) ≡ 1 (mod 3)}|, where p(k) is the
 *            partition function. The implementation computes the partition
 *            numbers modulo 3 by Euler’s pentagonal recurrence and keeps a
 *            running count of those congruent to 1. It stores the results in
 *            static arrays that are grown lazily and acces...
 * Range:     u8 n <= 826, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*--- static state --------------------------------------------------------*/
static unsigned char *p_mod   = NULL;   /* p(k) modulo 3, for k ≤ max_n */
static seq_t        *a_seq   = NULL;   /* cumulative counts a(k), k ≤ max_n */
static size_t        cap     = 0;       /* capacity in entries (≥ max_n+1) */
static size_t        max_n   = 0;       /* highest index for which the tables
                                         have been filled */

/*--- helpers ------------------------------------------------------------*/
static void ensure_capacity(size_t n)
{
    /* ensure that the tables have room for indices 0 … n inclusive */
    if (n + 1 <= cap) return;

    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap <= n) new_cap *= 2;

    unsigned char *new_p = (unsigned char *)realloc(p_mod, new_cap);
    seq_t        *new_a = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    if (!new_p || !new_a) abort();      /* out of memory */

    p_mod = new_p;
    a_seq = new_a;
    cap   = new_cap;
}

/* initialise tables up to index `n` (inclusive).  If `n==0` the tables
   remain untouched: a(0) is handled separately in `a`. */
static void compute_up_to(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= max_n) return;       /* already computed */

    ensure_capacity(need);

    /* first entry: p(0)=1, a(0)=1 (since 1≡1 (mod 3)) */
    if (max_n == 0 && cap > 0) {
        p_mod[0] = 1 % 3;
        a_seq[0] = 1;          /* a(0)=1 */
        max_n = 0;
    }

    /* compute successive partition numbers modulo 3 and accumulate a(k) */
    for (size_t i = max_n + 1; i <= need; ++i) {
        int sum = 0;
        for (int k = 1; ; ++k) {
            int pent1 = k * (3 * k - 1) / 2;
            int pent2 = k * (3 * k + 1) / 2;
            if (pent1 > (int)i && pent2 > (int)i) break;
            int sign = (k & 1) ? +1 : -1;
            if (pent1 <= (int)i) sum += sign * p_mod[i - pent1];
            if (pent2 <= (int)i) sum += sign * p_mod[i - pent2];
        }
        int mod_val = sum % 3;
        if (mod_val < 0) mod_val += 3;
        p_mod[i] = (unsigned char)mod_val;
        a_seq[i] = a_seq[i - 1] + ((p_mod[i] == 1) ? 1 : 0);
    }
    max_n = need;
}

/*--- public API ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 by definition */
    compute_up_to(n);
    return a_seq[(size_t)n];
}

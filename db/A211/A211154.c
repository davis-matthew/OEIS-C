/*
 * A211154  Number of 2 X 2 matrices having all terms in {-n,...,0,..,n} and
 *          even determinant.
 * https://oeis.org/A211154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the linear recurrence a(n) = a(n-1) + 4*a(n-2)
 *            - 4*a(n-3) - 6*a(n-4) + 6*a(n-5) + 4*a(n-6) - 4*a(n-7) - a(n-8)
 *            + a(n-9) for n > 9 Initial values: a(0)=1, a(1)=41, a(2)=457,
 *            a(3)=1345, a(4)=4481, a(5)=8521, a(6)=18985, a(7)=30017,
 *            a(8)=54721, a(9)=78121
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 143, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;   /* array of computed terms */
static idx_t seq_len   = 0;       /* number of terms currently stored (size) */
static size_t seq_cap  = 0;       /* allocated capacity of seq_cache */

/* Ensure the cache can hold at least (n+1) entries. */
static void ensure_capacity(idx_t n)
{
    if (n < (idx_t)seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 10UL;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Initialise the cache with the first 10 values. */
static void init_cache(void)
{
    if (seq_cache != NULL)
        return;
    seq_cap   = 10UL;
    seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_cache[0] = (seq_t)1;
    seq_cache[1] = (seq_t)41;
    seq_cache[2] = (seq_t)457;
    seq_cache[3] = (seq_t)1345;
    seq_cache[4] = (seq_t)4481;
    seq_cache[5] = (seq_t)8521;
    seq_cache[6] = (seq_t)18985;
    seq_cache[7] = (seq_t)30017;
    seq_cache[8] = (seq_t)54721;
    seq_cache[9] = (seq_t)78121;
    seq_len   = 10;
}

/* Return the n-th term of the sequence (a(0) is the first term). */
seq_t a(idx_t n)
{
    init_cache();

    if (n < seq_len)
        return seq_cache[n];

    ensure_capacity(n);

    for (idx_t i = seq_len; i <= n; ++i)
    {
        seq_t val = seq_cache[i-1];
        val += ((seq_t)4) * seq_cache[i-2];
        val -= ((seq_t)4) * seq_cache[i-3];
        val -= ((seq_t)6) * seq_cache[i-4];
        val += ((seq_t)6) * seq_cache[i-5];
        val += ((seq_t)4) * seq_cache[i-6];
        val -= ((seq_t)4) * seq_cache[i-7];
        val -= seq_cache[i-8];
        val += seq_cache[i-9];
        seq_cache[i] = val;
    }

    seq_len = n + 1;
    return seq_cache[n];
}

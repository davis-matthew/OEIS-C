/*
 * A104407  Number of Hamiltonian groups of order <= n.
 * https://oeis.org/A104407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1655, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∑_{k≤n} h(k), where h(8·2^k·m) = ∏_{p^e‖m} p(e),
 *              p(e) = number of partitions of e.
 *         h(k)=0 unless 8 divides k and the 2‑part of k is ≥8.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------- partitions of exponents (e ≤ 64) ----------- */
static seq_t part[65];
static bool part_ready = false;

static void init_partitions(void)
{
    if (part_ready) return;
    part[0] = 1;
    for (int i = 1; i < 65; ++i) part[i] = 0;
    for (int i = 1; i < 65; ++i)
        for (int j = i; j < 65; ++j)
            part[j] += part[j - i];
    part_ready = true;
}

/* ----------- smallest prime factor array (up to current size) ----------- */
static uint32_t *spf = NULL;
static idx_t spf_size = 0;

static void ensure_spf(idx_t n)
{
    if (spf && spf_size > 0 && spf_size - 1 >= n)
        return;

    size_t sz = (size_t)n + 1U;
    uint32_t *new_spf = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!new_spf) return;              /* allocation failure */

    for (idx_t i = 2; i * i <= n; ++i)
    {
        if (new_spf[i] == 0)
        {
            new_spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= n; j += i)
                if (new_spf[j] == 0)
                    new_spf[j] = (uint32_t)i;
        }
    }
    for (idx_t i = 2; i <= n; ++i)
        if (new_spf[i] == 0)
            new_spf[i] = (uint32_t)i;

    free(spf);
    spf = new_spf;
    spf_size = sz;
}

/* ----------- number of abelian groups of odd order q ----------- */
static seq_t count_abelian_odd(idx_t q)
{
    init_partitions();
    seq_t res = 1;
    while (q > 1)
    {
        uint32_t p = spf[q];
        uint32_t exp = 0;
        while (q % p == 0)
        {
            q /= p;
            ++exp;
        }
        res *= part[exp];
    }
    return res;
}

/* ----------- number of Hamiltonian groups of exact order n ----------- */
static seq_t h_exact(idx_t n)
{
    if (n < 8) return 0;              /* minimal order 8 */

    /* exponent of 2 in n */
    idx_t temp = n;
    int v2 = 0;
    while ((temp & 1ULL) == 0)
    {
        ++v2;
        temp >>= 1;
    }
    if (v2 < 3) return 0;             /* need at least 2^3 */

    idx_t odd = temp;                 /* odd part after removing all 2’s */
    return count_abelian_odd(odd);
}

/* ----------- cumulative sequence up to n ----------- */
static seq_t *a_table = NULL;
static idx_t a_last  = 0;              /* largest n already computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for zero or negative input */

    if (a_last < n)
    {
        /* Ensure auxiliary tables are large enough */
        ensure_spf(n);

        /* Allocate or grow cumulative table */
        size_t new_cap = (size_t)n + 1U;
        seq_t *new_tbl = (a_table == NULL)
                         ? (seq_t *)calloc(new_cap, sizeof(seq_t))
                         : (seq_t *)realloc(a_table, new_cap * sizeof(seq_t));
        if (!new_tbl)                /* allocation failed – return previous value */
            return a_table ? a_table[a_last] : 0;
        a_table = new_tbl;
        if (a_table[0] == 0) a_table[0] = 0;  /* zero initial entry */

        /* Compute values from a_last+1 up to n */
        for (idx_t i = a_last + 1; i <= n; ++i)
        {
            seq_t fi = h_exact(i);
            a_table[i] = a_table[i - 1] + fi;
        }
        a_last = n;
    }
    return a_table[n];
}

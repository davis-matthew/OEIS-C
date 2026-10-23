/*
 * A132666  a(1)=1, a(n) = 2*a(n-1) if the minimal positive integer not yet in
 *          the sequence is greater than a(n-1), else a(n) = a(n-1)-1.
 * https://oeis.org/A132666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132666
   a(1) = 1
   a(n) = 2*a(n-1)                 if the smallest unused positive integer
   a(n) = a(n-1)-1 (if free)       is ≤ a(n-1)
   otherwise double the previous term.
   This implements the recursion described in the OEIS entry.  */

/* --------------------------------------------------------------------------
   Standard includes – required for memory allocation.
   ------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------------
   Static state for memoisation.  We keep the sequence already computed
   and a disjoint–set union (DSU) structure that tracks used numbers.
   ------------------------------------------------------------- */
static seq_t *seq = NULL;          /* 1‑based array of terms               */
static idx_t  seq_cap = 0;         /* capacity in terms                    */
static idx_t  seq_len = 0;         /* last term computed (highest index)   */

static idx_t *par = NULL;          /* DSU parent array for used values     */
static unsigned char *used = NULL; /* bitmap of used numbers               */
static idx_t  par_cap = 0;         /* capacity of par/used arrays          */
static idx_t  min_unused = 1;      /* current smallest unused integer      */

/* --------------------------------------------------------------------------
   DSU helper functions.  All operations are performed on the 'par' array.
   ------------------------------------------------------------- */
static idx_t find_root(idx_t x)
{
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

/* Ensure that 'par' and 'used' have room for values up to 'needed'.
   New indices are initialised with parent = itself and used = 0.   */
static void ensure_value_capacity(idx_t needed)
{
    if (needed < par_cap) return;

    idx_t new_cap = par_cap ? par_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;

    par = (idx_t*)realloc(par, new_cap * sizeof(idx_t));
    if (!par) exit(1);
    used = (unsigned char*)realloc(used, new_cap * sizeof(unsigned char));
    if (!used) exit(1);

    for (idx_t i = par_cap; i < new_cap; ++i) {
        par[i] = i;
        used[i] = 0;
    }
    par_cap = new_cap;
}

/* Mark a value 'v' as used, updating the DSU and the global min_unused. */
static void use_value(idx_t v)
{
    ensure_value_capacity(v);
    if (!used[v]) {
        used[v] = 1;
        /* Merge v into the set of smaller numbers. */
        par[v] = find_root(v - 1);
    }

    /* Advance min_unused until it points to an unused number. */
    while (1) {
        ensure_value_capacity(min_unused);
        if (!used[min_unused]) break;
        ++min_unused;
    }
}

/* --------------------------------------------------------------------------
   Build the sequence up to index 'n'.  This may involve computing many
   values, but each is produced in O(1) amortised time.
   ------------------------------------------------------------- */
static void ensure_sequence_upto(idx_t n)
{
    if (n <= seq_len) return;

    /* Initialise on first call. */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);

        par_cap = 16;
        par = (idx_t*)malloc(par_cap * sizeof(idx_t));
        if (!par) exit(1);
        used = (unsigned char*)malloc(par_cap * sizeof(unsigned char));
        if (!used) exit(1);
        for (idx_t i = 0; i < par_cap; ++i) {
            par[i] = i;
            used[i] = 0;
        }
        par[0] = 0;  /* sentinel representing value 0 */

        /* First term */
        seq[1] = (seq_t)1;
        seq_len = 1;
        min_unused = 2;
        use_value(1);
    }

    /* Grow the sequence array if necessary. */
    while (seq_cap <= n) seq_cap <<= 1;
    seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
    if (!seq) exit(1);

    seq_t current = seq[seq_len];
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t next_val;
        if (min_unused > (idx_t)current) {
            /* Double if the smallest unused > current. */
            next_val = current * ((seq_t)2);
        } else {
            /* Skip to the largest unused number < current. */
            idx_t pred = find_root((idx_t)current - 1);
            next_val = (seq_t)pred;
        }
        seq[i] = next_val;
        use_value((idx_t)next_val);
        current = next_val;
    }
    seq_len = n;
}

/* --------------------------------------------------------------------------
   Required public function.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety for offset 1, not used */
    ensure_sequence_upto(n);
    return seq[n];
}

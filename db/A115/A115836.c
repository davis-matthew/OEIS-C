/*
 * A115836  Self-describing sequence. The n-th integer of the sequence
 *          indicates how many integers of the sequence are strictly < 2n.
 * https://oeis.org/A115836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the lexicographically earliest monotonic sequence * of
 *            positive integers satisfying a(a(n)+1) = 2*n. * * The following
 *            implementation builds the sequence on demand, * memoising
 *            results in dynamic arrays. It works for any seq_t type *
 *            supported by the build, including arbitrary‑precision i...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool */

static seq_t *seq      = NULL;   /* values, 1‑based index */
static unsigned char *assigned = NULL; /* 1 = known, 0 = unknown */
static idx_t cap = 0;

/* Ensure that the internal arrays can hold indices up to 'need'. */
static void ensure_cap(idx_t need)
{
    if (need <= cap) return;

    idx_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_seq = (seq_t *)realloc(seq, (size_t)(new_cap + 1) * sizeof(seq_t));
    unsigned char *new_assigned = (unsigned char *)realloc(
        assigned, (size_t)(new_cap + 1) * sizeof(unsigned char));

    if (!new_seq || !new_assigned)
        abort();

    /* initialise new slots */
    for (size_t i = (size_t)cap + 1; i <= (size_t)new_cap; ++i) {
        new_seq[i] = (seq_t)0;
        new_assigned[i] = 0;
    }

    seq = new_seq;
    assigned = new_assigned;
    cap = new_cap;
}

/* Initialise first term of the sequence. */
static void init(void)
{
    ensure_cap(1);
    seq[1] = (seq_t)1;
    assigned[1] = 1;
}

/* Return the nth term of A115836 (1‑based).  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */

    if (!seq) init();                /* initialise on first call */

    /* Compute terms up to n */
    for (idx_t cur = 2; cur <= n; ++cur) {
        if (!assigned[cur]) {        /* value not forced yet – take smallest */
            seq_t prev = seq[cur - 1];
            seq_t val   = prev + 1;  /* minimal integer strictly larger */
            seq[cur]    = val;
            assigned[cur] = 1;
        }

        /* Enforce the defining recurrence a(a(cur)+1) = 2*cur */
        seq_t val       = seq[cur];
        idx_t target    = (idx_t)val + 1;
        seq_t required  = (seq_t)cur * (seq_t)2;

        ensure_cap(target);
        if (!assigned[target]) {
            seq[target]   = required;
            assigned[target] = 1;
        } else if (seq[target] != required) {
            /* Inconsistent state – should never happen */
            abort();
        }
    }

    return seq[n];
}

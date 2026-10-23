/*
 * A121701  Lexicographically earliest sequence such that a(m)<>a(n) for all m
 *          with m<>n except either for m=2*n or n=2*m.
 * https://oeis.org/A121701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence where a(m)=a(n) only if m=2*n or n=2*m.
   The algorithm assigns the values greedily:  
   • for odd indices, use the smallest unused positive integer;  
   • for even indices, reuse the value a(n/2) if it has been used only once,  
     otherwise use the smallest unused integer.  This preserves the
   property that every value appears at most twice and only at indices
   that are double each other.  */
#include <stdlib.h>   /* malloc, realloc, free, abort */
#include <stdint.h>   /* uint64_t */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* Static storage for the sequence and the usage counts of each value */
static seq_t *seq = NULL;          /* 1‑indexed array of a(n) (0 = not yet set) */
static size_t seq_cap = 0;         /* maximum index allocated (>= n) */

static idx_t *val_count = NULL;    /* val_count[v] = 0/1/2 : occurrences of v */
static size_t val_cap = 0;         /* maximum value allocated (>= all v) */

static idx_t next_unused = 1;      /* smallest value with val_count[v]==0 */

/* Ensure seq[1..n] exists, initializing new entries to 0 */
static void ensure_seq(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    /* zero new slots */
    for (size_t i = seq_cap + 1; i <= new_cap; ++i) tmp[i] = (seq_t)0;
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure val_count[1..v] exists, initializing new entries to 0 */
static void ensure_val(size_t v)
{
    if (v <= val_cap) return;
    size_t new_cap = val_cap ? val_cap << 1 : 4;
    while (new_cap < v) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(val_count, (new_cap + 1) * sizeof(idx_t));
    if (!tmp) abort();
    /* zero new slots */
    for (size_t i = val_cap + 1; i <= new_cap; ++i) tmp[i] = 0;
    val_count = tmp;
    val_cap = new_cap;
}

/* Return the next available unused integer.  The returned value has
   val_count[value]==0.  */
static idx_t get_next_unused(void)
{
    while (next_unused <= val_cap && val_count[next_unused] != 0) {
        ++next_unused;
        ensure_val(next_unused);
    }
    if (next_unused > val_cap) {
        /* ensure capacity for the new index */
        ensure_val(next_unused);
    }
    return next_unused;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but not requested */

    ensure_seq((size_t)n);

    for (idx_t i = 1; i <= n; ++i) {
        if (seq[i] != (seq_t)0) continue;   /* already computed */

        if ((i & 1) == 0) {            /* even index: try to reuse a(i/2) */
            idx_t k = i >> 1;
            seq_t val_t = seq[k];
            idx_t val = (idx_t)val_t;
            ensure_val(val);
            if (val_count[val] == 1) {
                seq[i] = val_t;
                val_count[val] = 2;
                continue;
            }
            /* cannot reuse – need a new value */
            idx_t new_val = get_next_unused();
            seq[i] = (seq_t)new_val;
            val_count[new_val] = 1;
            ++next_unused;            /* next_unused now points to next trial */
            while (next_unused <= val_cap && val_count[next_unused] != 0) {
                ++next_unused;
                ensure_val(next_unused);
            }
        } else {                      /* odd index – must be new */
            idx_t new_val = get_next_unused();
            seq[i] = (seq_t)new_val;
            val_count[new_val] = 1;
            ++next_unused;
            while (next_unused <= val_cap && val_count[next_unused] != 0) {
                ++next_unused;
                ensure_val(next_unused);
            }
        }
    }

    return seq[n];
}

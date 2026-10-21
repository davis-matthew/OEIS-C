/*
 * A229119  a(n) is the (reversed lexicographic, alias Mathematica ordering)
 *          rank of the partition associated with integer n by encoding the
 *          run lengths of the binary representation of n.
 * https://oeis.org/A229119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the rank of the partition obtained from n; rank is
 *            computed in the ordering of partitions by sum ascending and
 *            lexicographically descending within each sum
 * Range:     u8 n <= 70, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#ifndef MAX_SUM
#define MAX_SUM 130   /* sufficient for 64‑bit n (max sum <= 126) */
#endif

/* Global table of partition counts p(s) for s = 0..MAX_SUM */
static seq_t partition_counts[MAX_SUM + 1];
static int partition_counts_initialized = 0;

/* Initialize partition_counts once */
static void init_partition_counts(void)
{
    int i, j;
    if (partition_counts_initialized) return;
    /* p(0) = 1; others initially 0 */
    for (i = 0; i <= MAX_SUM; ++i) partition_counts[i] = 0;
    partition_counts[0] = (seq_t)1;
    for (i = 1; i <= MAX_SUM; ++i) {
        for (j = i; j <= MAX_SUM; ++j) {
            partition_counts[j] += partition_counts[j - i];
        }
    }
    partition_counts_initialized = 1;
}

/* State used during recursive partition enumeration */
typedef struct {
    const seq_t *target;
    size_t tlen;
    unsigned int sum;
    seq_t rank_offset;
    unsigned int cand[MAX_SUM];
    unsigned int current_rank;
    seq_t result;
} RankState;

/* Recursive enumeration of integer partitions of remaining sum 'rem'.
   Parts are chosen in non‑increasing order.  When a partition equals
   the target, 'result' is filled and the function returns true. */
static int rec_partitions(RankState *st, unsigned int rem,
                          unsigned int max_part, unsigned int depth)
{
    if (rem == 0) {
        st->current_rank++;
        if (depth == st->tlen) {
            size_t k;
            for (k = 0; k < st->tlen; ++k) {
                if (st->cand[k] != (unsigned int)st->target[k]) return 0;
            }
            st->result = st->rank_offset + st->current_rank;
            return 1;
        }
        return 0;
    }
    if (max_part > rem) max_part = rem;
    for (unsigned int part = max_part; part >= 1; --part) {
        st->cand[depth] = part;
        if (rec_partitions(st, rem - part, part, depth + 1)) return 1;
        if (part == 1) break; /* unsigned underflow guard */
    }
    return 0;
}

/* Wrapper that returns the rank of partition `target` of length `tlen`
   and total sum `sum`, using rank_offset for preceding sums. */
static seq_t find_rank(const seq_t *target, size_t tlen,
                       unsigned int sum, seq_t rank_offset)
{
    RankState st = { target, tlen, sum, rank_offset, {0}, 0, 0 };
    rec_partitions(&st, sum, sum, 0);
    return st.result;
}

/* Main function: a(n) */
seq_t a(idx_t n)
{
    unsigned int runs[MAX_SUM];
    size_t r_count = 0;
    unsigned int bitlen = 0;
    idx_t tmp = n;

    /* Compute bit length of n (n>0) */
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Build run lengths of alternating 1‑ and 0‑blocks in binary of n */
    unsigned int expecting = 1;
    unsigned int run_len = 0;
    size_t i;
    for (i = bitlen; i > 0; --i) {
        unsigned int bit = (n >> (i - 1)) & 1;
        if (bit == expecting) {
            ++run_len;
        } else {
            runs[r_count++] = run_len;
            run_len = 1;
            expecting = bit;
        }
    }
    runs[r_count++] = run_len;  /* last run */

    /* Subtract 1 from all runs except the last */
    for (size_t j = 0; j + 1 < r_count; ++j) {
        if (runs[j] == 0) /* should not happen but guard */
            runs[j] = 0;
        else
            --runs[j];
    }

    /* Reverse run lengths */
    unsigned int rev[MAX_SUM];
    for (size_t j = 0; j < r_count; ++j) {
        rev[j] = runs[r_count - 1 - j];
    }

    /* Running sum of reversed runs */
    for (size_t j = 1; j < r_count; ++j) {
        rev[j] += rev[j - 1];
    }

    /* Build the partition by reversing again */
    seq_t part[MAX_SUM];
    size_t part_len = r_count;
    seq_t sum = 0;
    for (size_t j = 0; j < r_count; ++j) {
        unsigned int val = rev[r_count - 1 - j];
        part[j] = (seq_t)val;
        sum += (seq_t)val;
    }

    /* Ensure partition_counts table is ready */
    if (!partition_counts_initialized) init_partition_counts();

    /* Compute rank offset: number of partitions with sum < current sum */
    unsigned int sum_int = (unsigned int)sum;   /* safe: sum <= 128 */
    seq_t rank_offset = 0;
    for (unsigned int s = 1; s < sum_int; ++s) {
        rank_offset += partition_counts[s];
    }

    /* Find rank of the partition within its sum group */
    return find_rank(part, part_len, sum_int, rank_offset);
}

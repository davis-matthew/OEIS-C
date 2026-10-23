/*
 * A176884  Beginning with 2, the distance between a number and the previous
 *          number with same number of prime factors, repetitions included.
 *          Powers of 2 are assigned zero. See example.
 * https://oeis.org/A176884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = distance between the number (n+1) and the previous number
 *            with the same count of prime factors (with repetitions). Powers
 *            of two are assigned 0
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   bigomega, ensure_capacity, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_power_of_two.h"
#include "../utils/bigomega.h"
#include "../utils/ensure_capacity.h"

static seq_t *a_values = NULL;   /* a_values[num] = a(n) where num = n+1 */
static size_t a_cap = 0;
static idx_t last_computed = 1;  /* last number processed (initially 1) */

static seq_t *last_occ = NULL;   /* last_occ[omega] = last number with that omega */
static size_t last_cap = 0;

/* Ensure that the array *arr has at least 'needed' elements, reallocating
   as necessary. New elements are zero‑initialized. */
static void ensure_cap(size_t *cap, seq_t **arr, size_t needed)
{
    if (*cap >= needed)
        return;
    ensure_capacity(needed, cap, (void **)arr, sizeof(seq_t));
    for (size_t i = *cap; i < needed; ++i)
        (*arr)[i] = 0;
}

static void compute_upto(idx_t up)
{
    if (up <= last_computed)
        return;
    /* Ensure enough storage for a_values up to index 'up' */
    ensure_cap(&a_cap, &a_values, (size_t)up + 1);

    for (idx_t num = last_computed + 1; num <= up; ++num) {
        seq_t result = 0;
        if (!is_power_of_two(num)) {
            seq_t omega = bigomega(num);          /* omega fits in seq_t */
            idx_t o = (idx_t)omega;              /* index for last_occ */
            if (o >= last_cap)
                ensure_cap(&last_cap, &last_occ, o + 1);
            seq_t prev = last_occ[o];
            if (prev != 0)
                result = (seq_t)(num - prev);
            last_occ[o] = num;                 /* record this as last */
        } else {
            /* powers of two get a result of 0, but still record omega */
            seq_t omega = bigomega(num);
            idx_t o = (idx_t)omega;
            if (o >= last_cap)
                ensure_cap(&last_cap, &last_occ, o + 1);
            last_occ[o] = num;
            result = 0;
        }
        a_values[num] = result;
    }
    last_computed = up;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* a(1) corresponds to number 2 */
    idx_t number = n + 1;
    compute_upto(number);
    return a_values[number];
}

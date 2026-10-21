/*
 * A209186  Admission order to the USA of the n-th state in alphabetical
 *          order.
 * https://oeis.org/A209186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sort the 50 US state names by alphabetical order and
   return the admission ordinal of the nth state in this order. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const char *states_by_admission[50] = {
    "Delaware", "Pennsylvania", "New Jersey", "Georgia", "Connecticut",
    "Massachusetts", "Maryland", "South Carolina", "New Hampshire", "Virginia",
    "New York", "North Carolina", "Rhode Island", "Vermont", "Kentucky",
    "Tennessee", "Ohio", "Louisiana", "Indiana", "Mississippi",
    "Illinois", "Alabama", "Maine", "Missouri", "Arkansas",
    "Michigan", "Florida", "Texas", "Iowa", "Wisconsin",
    "California", "Minnesota", "Oregon", "Kansas", "West Virginia",
    "Nevada", "Nebraska", "Colorado", "North Dakota", "South Dakota",
    "Montana", "Washington", "Idaho", "Wyoming", "Utah",
    "Oklahoma", "New Mexico", "Arizona", "Alaska", "Hawaii"
};

static idx_t sorted_idx[50];
static bool sorted_initialized = false;

static int compare_states(const void *p1, const void *p2)
{
    const idx_t *i = (const idx_t *)p1;
    const idx_t *j = (const idx_t *)p2;
    return strcmp(states_by_admission[*i], states_by_admission[*j]);
}

static void ensure_sorted(void)
{
    if (!sorted_initialized) {
        for (idx_t k = 0; k < 50; ++k) sorted_idx[k] = k;
        qsort(sorted_idx, 50, sizeof(idx_t), compare_states);
        sorted_initialized = true;
    }
}

seq_t a(idx_t n)
{
    if (n < 1 || n > 50) return 0;
    ensure_sorted();
    /* Admission ordinal is index + 1 in the original admission order */
    return (seq_t)(sorted_idx[n - 1] + 1);
}

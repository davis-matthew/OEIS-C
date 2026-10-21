/*
 * A051415  Numbers not of form n + sum of digits of n^3.
 * https://oeis.org/A051415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_sum_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051415: Numbers not of the form n + sum_of_digits(n³) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_u128.h"

/* static state – maintained between calls */
static bool *represent = NULL;          /* flags for representable numbers */
static size_t rep_size = 0;             /* current size of represent[] (index 0 unused) */
static size_t next_m = 1;               /* next m to be processed */

static unsigned long long *missing = NULL;   /* list of missing numbers (up to current scan) */
static size_t missing_size = 0;              /* number of missing values stored */
static size_t missing_cap = 0;               /* allocated capacity of missing[] */

static size_t last_scan = 0;            /* highest number already scanned for missingness */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*  Expand represent[] to at least new_size (indexable up to new_size) */
static void enlarge_rep(size_t new_size)
{
    if (new_size <= rep_size)
        return;
    /* reallocate (C++ cast required) */
    bool *tmp = (bool *)realloc(represent, (new_size + 1) * sizeof(bool));
    for (size_t i = rep_size + 1; i <= new_size; ++i)
        tmp[i] = false;
    represent = tmp;
    rep_size = new_size;
}

/* ------------------------------------------------------------ */
/*  Process m values from next_m up to up_to (inclusive)
    marking their f(m) = m + sum_of_digits(m³) when <= rep_size  */
static void process_m_values(size_t up_to)
{
    for (size_t m = next_m; m <= up_to; ++m) {
        unsigned __int128 m3 = (unsigned __int128)m * m * m;
        uint32_t sum = digit_sum_u128(m3);
        unsigned __int128 val = (unsigned __int128)m + sum;
        if (val <= rep_size)
            represent[(size_t)val] = true;
    }
    next_m = up_to + 1;
}

/* ------------------------------------------------------------ */
/*  Grow missing[] to hold at least needed additional items */
static void grow_missing(size_t needed)
{
    if (missing_cap >= needed)
        return;
    size_t new_cap = missing_cap ? missing_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    unsigned long long *tmp = (unsigned long long *)realloc(missing, new_cap * sizeof(unsigned long long));
    missing = tmp;
    missing_cap = new_cap;
}

/* ------------------------------------------------------------ */
/*  Scan numbers from last_scan+1 up to limit
    and store those that are not representable */
static void gather_missing_up_to(size_t limit)
{
    if (last_scan == limit)
        return;
    size_t required = missing_size + (limit - last_scan);
    grow_missing(required);
    for (size_t k = last_scan + 1; k <= limit; ++k) {
        if (!represent[k]) {
            missing[missing_size++] = k;
        }
    }
    last_scan = limit;
}

/* ------------------------------------------------------------ */
/*  Public function a(idx_t n) – zero‑based offset is 1 (a(1)=1)   */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset < 1 */
        return 0;

    while (missing_size < (size_t)n) {
        /* grow represent[] heuristically */
        size_t target = rep_size ? rep_size * 2 : 1024;
        if (target < (size_t)(n * 2))
            target = n * 2;
        enlarge_rep(target);

        /* process all m up to the new represent size */
        process_m_values(rep_size);

        /* scan numbers up to the represent size to find new missing values */
        gather_missing_up_to(rep_size);
    }

    /* cast the stored 64‑bit value to the expected seq_t type */
    return (seq_t)missing[n - 1];
}

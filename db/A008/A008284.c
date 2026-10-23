/*
 * A008284  Triangle of partition numbers: T(n,k) = number of partitions of n
 *          in which the greatest part is k, 1 <= k <= n. Also number of
 *          partitions of n into k positive parts, 1 <= k <= n.
 * https://oeis.org/A008284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 330, u16 n <= 1780, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008284: Triangle of partition numbers.  
   T(n, k) = 1,             if k == 1 or k == n;  
   T(n, k) = T(n-1, k-1) + T(n-k, k),  otherwise.  
   The sequence a(n) is the row‑major flattening of this triangle:
   a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), … */

#include <stddef.h>
#include <stdlib.h>

/* Helper: allocate an array of seq_t of given size.
   In C++ use new[], in C use malloc. */
#ifdef __cplusplus
static seq_t* _alloc_seq(size_t sz) { return new seq_t[sz]; }
static void      _free_seq(seq_t* p) { delete[] p; }
#else
static seq_t* _alloc_seq(size_t sz) { return (seq_t*)malloc(sz * sizeof(seq_t)); }
static void     _free_seq(seq_t* p) { free(p); }
#endif

/* ----------  Partition numbers p[n]  ------------------- */
static seq_t *partitions = NULL;      /* p[0..N] */
static size_t partitions_size = 0;    /* number of entries */

/* Allocate and compute partition numbers up to n (inclusive). */
static void compute_partitions_to(size_t n)
{
    seq_t *newPart = _alloc_seq(n + 1);
    for (size_t i = 0; i <= n; ++i) newPart[i] = 0;
    newPart[0] = 1;                          /* p(0) = 1 */

    for (size_t i = 1; i <= n; ++i) {
        for (size_t j = i; j <= n; ++j)
            newPart[j] = newPart[j] + newPart[j - i];
    }

    if (partitions) _free_seq(partitions);
    partitions      = newPart;
    partitions_size = n + 1;
}

/* ----------  Triangle of T(n,k)  ----------------- */
static seq_t *triangle   = NULL;   /* flattened array */
static size_t triangle_row = 0;    /* rows already computed (row 0 means none) */

/* Allocate a new triangle array and copy old contents. */
static void expand_triangle(size_t new_total, size_t old_total)
{
    seq_t *newTri = _alloc_seq(new_total);
    for (size_t i = 0; i < old_total; ++i) newTri[i] = triangle[i];
    if (triangle) _free_seq(triangle);
    triangle = newTri;
}

/* Ensure that rows 1..target_row have been computed. */
static void ensure_triangle_up_to(size_t target_row)
{
    if (target_row <= triangle_row) return;

    /*  Compute required partition numbers.  */
    size_t max_part_needed = target_row / 2;            /* floor(row/2) */
    if (partitions_size <= max_part_needed)
        compute_partitions_to(max_part_needed);

    /*  Resize the triangle array to hold all entries up to target_row. */
    size_t old_total = triangle_row * (triangle_row + 1) / 2;
    size_t new_total = target_row * (target_row + 1) / 2;
    if (!triangle) {
        triangle = _alloc_seq(new_total);
    } else {
        expand_triangle(new_total, old_total);
    }

    /*  Compute new rows. */
    for (size_t r = triangle_row + 1; r <= target_row; ++r) {
        size_t offset = (r - 1) * r / 2;               /* first index of row r (k=1) */
        size_t ceil_half = (r + 1) / 2;                /* ceil(r/2) */

        for (size_t k = 1; k <= r; ++k) {
            seq_t val;
            if (k == 1 || k == r) {
                val = 1;
            } else if (k >= ceil_half) {
                val = partitions[r - k];    /* T(r,k) = p(r-k) */
            } else {
                /* T(r-1,k-1) */
                size_t idx1 = ((r - 2) * (r - 1)) / 2 + (k - 2);
                /* T(r-k,k) */
                size_t idx2 = ((r - k - 1) * (r - k)) / 2 + (k - 1);
                val = triangle[idx1] + triangle[idx2];
            }
            triangle[offset + (k - 1)] = val;
        }
    }

    triangle_row = target_row;
}

/* Convert linear index n (>=1) into row and column in the triangle. */
static void index_to_row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* Binary search for smallest r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= n) high = mid;
        else                          low = mid + 1;
    }
    *row = low;
    idx_t first_index_of_row = (*row) * ((*row) - 1) / 2;
    *col = n - first_index_of_row;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */

    idx_t row, col;
    index_to_row_col(n, &row, &col);

    ensure_triangle_up_to((size_t)row);

    size_t idx = ((size_t)row - 1) * (size_t)row / 2 + (size_t)(col - 1);
    return triangle[idx];
}

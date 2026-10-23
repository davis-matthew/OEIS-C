/*
 * A105105  Triangle read by rows, based on the morphism f: 1-> 2->3
 *          3->{6,5,4} 4->5 5->6 6->{3,2,1}. First row is 1. If current row is
 *          a,b,c,..., then the next row is a,b,c,...,f(a),f(b),f(c),...
 * https://oeis.org/A105105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A105105. Triangle read by rows; first row 1. If the current row
 *            is a,b,c,…, then the next row is a,b,c,…,f(a),f(b),f(c),… Where
 *            f is the morphism 1 → 2 2 → 3 3 → 6,5,4 4 → 5 5 → 6 6 → 3,2,1
 *            Off‑set 0. Returns the n‑th term
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* ----------------- C++ implementation with std::vector ----------------- */
#ifdef __cplusplus
    std::vector<uint8_t> row(1,1);   /* current row, values 1..6 */
    idx_t counter = 0;               /* number of elements already output */

    while (1) {
        /* Scan the current row */
        for (size_t i = 0; i < row.size(); ++i) {
            if (counter == n) return (seq_t)row[i];
            ++counter;
        }

        /* Build the next row: prefix + morphism of the current row */
        std::vector<uint8_t> next;
        next.reserve(row.size() * 4);                /* safe upper bound */
        next.insert(next.end(), row.begin(), row.end());  /* copy prefix */

        for (uint8_t v : row) {
            switch (v) {
                case 1: next.push_back(2); break;
                case 2: next.push_back(3); break;
                case 3: next.push_back(6); next.push_back(5); next.push_back(4); break;
                case 4: next.push_back(5); break;
                case 5: next.push_back(6); break;
                case 6: next.push_back(3); next.push_back(2); next.push_back(1); break;
            }
        }
        row.swap(next);   /* now 'row' holds the new row */
    }
#else
    /* ----------------- C implementation with malloc/realloc ----------------- */
    unsigned char *row = (unsigned char*)malloc(1);  /* first row: [1] */
    if (!row) return (seq_t)0;                      /* allocation failure */
    row[0] = 1;
    size_t row_len = 1;
    idx_t counter = 0;

    while (1) {
        /* Scan the current row */
        for (size_t i = 0; i < row_len; ++i) {
            if (counter == n) {
                seq_t val = row[i];
                free(row);
                return val;
            }
            ++counter;
        }

        /* Compute the length of the morphism part */
        size_t add_len = 0;
        for (size_t i = 0; i < row_len; ++i) {
            unsigned char v = row[i];
            add_len += (v == 3 || v == 6) ? 3 : 1;
        }
        size_t new_len = row_len + add_len;

        /* Reallocate to hold the new row (prefix + morphism) */
        unsigned char *next = (unsigned char*)realloc(row, new_len);
        if (!next) {
            free(row);
            return (seq_t)0;
        }
        /* The prefix is already preserved by realloc; write the morphism part after it */
        size_t pos = row_len;
        for (size_t i = 0; i < row_len; ++i) {
            unsigned char v = next[i];
            switch (v) {
                case 1: next[pos++] = 2; break;
                case 2: next[pos++] = 3; break;
                case 3: next[pos++] = 6; next[pos++] = 5; next[pos++] = 4; break;
                case 4: next[pos++] = 5; break;
                case 5: next[pos++] = 6; break;
                case 6: next[pos++] = 3; next[pos++] = 2; next[pos++] = 1; break;
            }
        }
        row = next;
        row_len = new_len;
    }
#endif
}
